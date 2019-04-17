/*****************************************************************************
Author      :   Paul Maximilian Bittner
/*****************************************************************************/

#include <iostream>
#include <fstream>

#include <algorithm>
#include <future>

#include <map>

#include <glm/glm.hpp>
#include <cor/CoRCalculator.h>
#include <cor/CoRTriangle.h>
#include <cor/CoRMesh.h>
#include <cor/Clock.h>

namespace CoR {
	CoRCalculator::CoRCalculator(
			float sigma,
			float omega,
			bool subdivide,
			unsigned int numberOfThreadsToCreate)
			: _sigma(sigma), _omega(omega), _subdivide(subdivide), _numThreads(numberOfThreadsToCreate)
	{
	}

	std::vector<WeightsPerBone> CoRCalculator::convertWeights(
			unsigned int numBones,
			const std::vector<std::vector<unsigned int>>& skeletonBoneIndices,
			const std::vector<std::vector<float>>& skeletonBoneWeights) const
	{
		std::vector<WeightsPerBone> weights(skeletonBoneWeights.size(), WeightsPerBone(numBones));

		for (int i = 0; i < skeletonBoneWeights.size(); ++i) {
			const std::vector<unsigned int>& indices = skeletonBoneIndices[i];
			const std::vector<float>& weightsToConvert = skeletonBoneWeights[i];
			WeightsPerBone &weightsToSet = weights[i];

			for (int weightIndex = 0; weightIndex < indices.size(); ++weightIndex) {
				int boneIndex = indices[weightIndex];
				weightsToSet[boneIndex] = weightsToConvert[weightIndex];
			}
		}

		return weights;
	}

	bool CoRCalculator::calculateCoR(unsigned long vertex, const CoRMesh &mesh, glm::vec3* corOut) const
	{
		WeightsPerBone weight = mesh.weights[vertex];

		glm::vec3 numerator(0);
		float denominator = 0;

		for (const CoRTriangle &t : mesh.triangles) {
			float sim = similarity(weight, t.averageWeight, _sigma);

			float areaTimesSim = t.area*sim;
			numerator += areaTimesSim * t.center;
			denominator += areaTimesSim;
		}

		//p_i^*
		glm::vec3 cor(0);
		if (denominator != 0)
			cor = numerator / denominator;
		*corOut = cor;

		return denominator != 0; // <=> has cor

	}

	void CoRCalculator::calculateCoRs_Interval(unsigned long from, unsigned long to, const CoRMesh &mesh, std::vector<glm::vec3>& cors) const
	{
		for (unsigned long i = from; i < to; ++i) {
			calculateCoR(i, mesh, cors.data() + i);
		}
	}

	void CoRCalculator::calculateCoRsAsync(const CoRMesh & mesh, const std::function<void(std::vector<glm::vec3>&)> & callback)
	{
		_worker = std::async([this, mesh, callback]
		{
			using namespace std::chrono;

#ifdef COR_ENABLE_PROFILING
			Clock globalClock;
			globalClock.clockStart();
#endif

			unsigned long vertexCount = mesh.vertices.size();

			// calculate CoRs
			std::vector<glm::vec3> cors(vertexCount, glm::vec3(0));
			{
				std::vector<std::future<void>> threads;
				auto intervall = static_cast<unsigned long>(std::ceil(static_cast<double>(vertexCount) / static_cast<double>(_numThreads)));

#ifdef COR_ENABLE_PROFILING
				std::cout << "Starting " << _numThreads << " threads with intervall = " << intervall << std::endl;
#endif

				for (unsigned long from = 0; from < vertexCount; from += intervall) {
					unsigned long to = std::min(from + intervall, vertexCount);
					threads.push_back(
							std::async(std::launch::async, &CoR::CoRCalculator::calculateCoRs_Interval, this, from, to, std::ref(mesh), std::ref(cors))
							);
				}
			} // End of scrope destroys vector of threads, whose destructors force us to wait until they finished execution

#ifdef COR_ENABLE_PROFILING
			globalClock.clockMessageAtCurrentTime("Whole computation took");
#endif

			callback(cors);
		});
	}

	void CoRCalculator::saveCoRsToBinaryFile(const std::string & path, std::vector<glm::vec3>& cors) const
	{
		unsigned long corCount = cors.size();

		std::ofstream outputFile;
		outputFile.open(path, std::ios::out | std::ios::binary);
		outputFile << corCount;
		outputFile.write(reinterpret_cast<char*>(cors.data()), 3 * sizeof(float) * cors.size());
		outputFile.close();
	}

	void CoRCalculator::saveCoRsToTextFile(const std::string &path, std::vector<glm::vec3> &cors, const std::string & separator) const {
		unsigned long corCount = cors.size();

		std::ofstream outputFile;
		outputFile.open(path, std::ios::out);
		outputFile << std::to_string(corCount) << std::endl;
		for (auto &cor : cors) {
			outputFile << std::to_string(cor.x) << separator << std::to_string(cor.y) << separator << std::to_string(cor.z) << "\n";
		}
		outputFile.flush();
		outputFile.close();
	}

	std::vector<glm::vec3> CoRCalculator::loadCoRsFromBinaryFile(const std::string & path) const
	{
		unsigned int corCount = 0;

		std::ifstream file(path, std::ifstream::in | std::ifstream::binary);
		file >> corCount;
		std::vector<glm::vec3> cors(corCount, glm::vec3(0.5));
		file.read(reinterpret_cast<char*>(cors.data()), 3 * sizeof(float) * corCount);

		return cors;
	}

	CoRMesh CoRCalculator::createCoRMesh(
			const std::vector<glm::vec3>& vertices,
			const std::vector<unsigned int>& indices,
			std::vector<WeightsPerBone>& skeletonBoneWeights,
			float subdivEpsilon) const
	{
		std::vector<glm::vec3> sub_vertices = vertices;
		std::vector<unsigned int> sub_triangleIndices = indices;
		std::vector<WeightsPerBone> sub_weights = skeletonBoneWeights;

		// subdivision
		if (_subdivide) {
#ifdef COR_ENABLE_PROFILING
			std::cout << "\tSubdividing mesh";
			Clock subDivClock;
			subDivClock.clockStart();
#endif
			for (int t = 0; t < sub_triangleIndices.size(); t += 3) {
				for (int edgeBeg = 0; edgeBeg < 3; ++edgeBeg) {
					int edgeEnd = (edgeBeg + 1) % 3;

					int globalEdgeBeg = t + edgeBeg;
					int globalEdgeEnd = t + edgeEnd;

					unsigned int v0Index = sub_triangleIndices[globalEdgeBeg];
					unsigned int v1Index = sub_triangleIndices[globalEdgeEnd];

					WeightsPerBone &w0 = sub_weights[v0Index];
					WeightsPerBone &w1 = sub_weights[v1Index];

					if (skinningWeightsDistance(w0, w1) >= subdivEpsilon) {
						// split edge into halfs

						// 1.) calc new vertex
						glm::vec3 v0 = sub_vertices[v0Index];
						glm::vec3 v1 = sub_vertices[v1Index];
						glm::vec3 newVertex = 0.5f * (v0 + v1);
						unsigned int newVertexIndex = sub_vertices.size();
						sub_vertices.push_back(newVertex);

						// 2.) calc new weight by linear interpolating
						sub_weights.push_back((w0 + w1) * 0.5f);

						// 3.) split triangle into two new ones
						unsigned int v2Index = sub_triangleIndices[t + ((edgeEnd + 1) % 3)];
						// append first new triangle at end of triangle list
						sub_triangleIndices.push_back(v0Index);
						sub_triangleIndices.push_back(v2Index);
						sub_triangleIndices.push_back(newVertexIndex);

						// replace current triangle with second new one and ...
						sub_triangleIndices[globalEdgeBeg] = newVertexIndex;
						//    => sub_triangleIndices[globalEdgeEnd]   = globalEdgeEnd;
						//    => sub_triangleIndices[lastVertexIndex] = lastVertexIndex;
						// ...recheck it
						t -= 3;
						break;
					}
				}
			}
#ifdef COR_ENABLE_PROFILING
			subDivClock.clockMessageAtCurrentTime("Mesh subdivision took");
#endif
		}

		CoRMesh mesh(
				sub_vertices,
				std::vector<CoRTriangle>(sub_triangleIndices.size() / 3, CoRTriangle()),
				sub_weights
		);

		// convert triangle indices to triangles and precompute cor values
		convertTriangles(sub_triangleIndices, &mesh);

		return mesh;
	}

	void CoRCalculator::convertTriangles(std::vector<unsigned int> triangleIndices, CoRMesh *mesh) const
	{
#ifdef COR_ENABLE_PROFILING
		Clock clock;
		clock.clockStart();
#endif

		std::vector<glm::vec3> &vertices = mesh->vertices;
		std::vector<CoRTriangle> &triangles = mesh->triangles;
		std::vector<WeightsPerBone> &weights = mesh->weights;

		for (int i = 0; i < triangles.size(); ++i) {
			CoRTriangle &t = triangles[i];

			int tIndex = 3 * i;
			t.alpha = triangleIndices[tIndex];
			t.beta = triangleIndices[tIndex + 1];
			t.gamma = triangleIndices[tIndex + 2];

			glm::vec3 vAlpha = vertices[t.alpha];
			glm::vec3 vBeta = vertices[t.beta];
			glm::vec3 vGamma = vertices[t.gamma];

			t.center = (vAlpha + vBeta + vGamma) * (1.0f / 3.0f);
			t.averageWeight = (weights[t.alpha] + weights[t.beta] + weights[t.gamma]) * (1.0f / 3.0f);

			glm::vec3 sideAB = vBeta - vAlpha;
			glm::vec3 sideAC = vGamma - vAlpha;
			t.area = 0.5f * glm::length(glm::cross(sideAB, sideAC));// abs?
		}

#ifdef COR_ENABLE_PROFILING
		clock.clockMessageAtCurrentTime("Triangle conversion took");
#endif
	}

	BFSCoRCalculator::BFSCoRCalculator(
			float sigma,
			float omega,
			bool subdivide,
			unsigned int numberOfThreadsToCreate,
			float bfsEpsilon) : CoRCalculator(sigma, omega, subdivide, numberOfThreadsToCreate), _bfsEpsilon(bfsEpsilon)
	{

	}

	void BFSCoRCalculator::convertTriangles(std::vector<unsigned int> triangleIndices, CoRMesh * mesh) const
	{
		CoRCalculator::convertTriangles(triangleIndices, mesh);
		std::vector<CoRTriangle> &triangles = mesh->triangles;

		unsigned long vertexCount = mesh->vertices.size();
		mesh->trianglesOfVertex = std::vector<std::vector<CoRTriangle*>>(vertexCount, std::vector<CoRTriangle*>());
		mesh->verticesWithSimilarSkinningWeights = std::vector<std::vector<int>>(vertexCount, std::vector<int>());

		for (auto &t : triangles) {
			// add t to triangles of vertices alpha, beta, gamma
			mesh->trianglesOfVertex[t.alpha].push_back(&t);
			mesh->trianglesOfVertex[t.beta].push_back(&t);
			mesh->trianglesOfVertex[t.gamma].push_back(&t);
		}

		calculateANNData(mesh, _omega);
	}

	bool BFSCoRCalculator::calculateCoR(unsigned long vertex, const CoRMesh & mesh, glm::vec3 * corOut) const
	{
		WeightsPerBone weight = mesh.weights[vertex];

		// find triangles for bfs
		std::vector<CoRTriangle*> trianglesToVisit;
		for (int homie : mesh.verticesWithSimilarSkinningWeights[vertex]) {
			trianglesToVisit.insert(
					trianglesToVisit.end(),
					mesh.trianglesOfVertex.at(homie).begin(),
					mesh.trianglesOfVertex.at(homie).end());
		}

		std::map<CoRTriangle*, bool> visited;
		glm::vec3 numerator(0);
		float denominator = 0;

		for (int i = 0; i < trianglesToVisit.size(); ++i) {
			CoRTriangle* t = trianglesToVisit[i];

			if (visited[t])
				continue;
			visited[t] = true;

			float sim = similarity(weight, t->averageWeight, _sigma);
			if (sim >= _bfsEpsilon) {
				float areaTimesSim = t->area*sim;
				numerator += areaTimesSim * t->center;
				denominator += areaTimesSim;

				for (CoRTriangle *neighbour : t->_neighbours) {
					if (!visited[neighbour])
						trianglesToVisit.push_back(neighbour);
				}
			}
		}

		//p_i^*
		glm::vec3 cor(0);
		if (denominator != 0)
			cor = numerator / denominator;
		*corOut = cor;

		return denominator != 0; // <=> has cor
	}

	void BFSCoRCalculator::calculateANNData(CoRMesh * mesh, float omega) const
	{
		unsigned long vertexCount = mesh->vertices.size();
#ifdef COR_ENABLE_PROFILING
		std::cout << "\tCache vertices with similar skinning weights and triangle neighbourhood" << std::endl;
		Clock clock;
		clock.clockStart();
#endif

		for (int i = 0; i < vertexCount; ++i) {
			WeightsPerBone &wi = mesh->weights[i];
			for (int j = i + 1; j < vertexCount; ++j) {
				WeightsPerBone &wj = mesh->weights[j];
				if (skinningWeightsDistance(wi, wj) < omega) {
					mesh->verticesWithSimilarSkinningWeights[i].push_back(j);
					mesh->verticesWithSimilarSkinningWeights[j].push_back(i);
				}
			}

			// find triangle neighbourhood
			std::vector<CoRTriangle*> &incidentTriangles = mesh->trianglesOfVertex[i];
			for (int t0 = 0; t0 < incidentTriangles.size(); ++t0) {
				for (int t1 = t0 + 1; t1 < incidentTriangles.size(); ++t1) {
					CoRTriangle *triangle0 = incidentTriangles[t0];
					CoRTriangle *triangle1 = incidentTriangles[t1];

					if (triangle0->tryAddNeighbour(*triangle1)) {
						triangle1->_neighbours.push_back(triangle0);
					}
				}
			}
		}

#ifdef COR_ENABLE_PROFILING
		clock.clockMessageAtCurrentTime("Calculation of BFS data took");
#endif
	}

}