/*****************************************************************************
Author      :   Paul Maximilian Bittner
/*****************************************************************************/

#ifndef COR_CORCALCULATOR_H
#define COR_CORCALCULATOR_H

#include <string>
#include <vector>
#include <future>

#include "WeightsPerBone.h"
#include "CoRMesh.h"

namespace CoR {
	class CoRCalculator {
		std::future<void> _worker;

	protected:
		// cor computation values
		float _sigma;
		float _omega;

		int _numThreads;
		bool _subdivide;

		// pre-calculation
		virtual void convertTriangles(std::vector<unsigned int> triangleIndices, CoRMesh *mesh) const;

        // cor computation
        virtual bool calculateCoR(unsigned long vertex, const CoRMesh &mesh, glm::vec3* corOut) const;
        // [from, to)
        void calculateCoRs_Interval(unsigned long from, unsigned long to, const CoRMesh &mesh, std::vector<glm::vec3>& cors) const;

	public:
		explicit CoRCalculator(
				float sigma = 0.1f,
				float omega = 0.1f,
				bool subdivide = true,
				unsigned int numberOfThreadsToCreate = 4);

		std::vector<WeightsPerBone> convertWeights(unsigned int numBones,
												   const std::vector<std::vector<unsigned int>>& skeletonBoneIndices,
												   const std::vector<std::vector<float>>& skeletonBoneWeights) const;

		CoRMesh createCoRMesh(
				const std::vector<glm::vec3>& vertices,
				const std::vector<unsigned int>& indices,
				std::vector<WeightsPerBone>& skeletonBoneWeights,
				float subdivEpsilon = 0.1f) const;

		void calculateCoRsAsync(const CoRMesh & mesh, const std::function<void(std::vector<glm::vec3>&)> & callback);

		// IO
		void saveCoRsToBinaryFile(const std::string & path, std::vector<glm::vec3>& cors) const;
		void saveCoRsToTextFile(const std::string & path, std::vector<glm::vec3>& cors, const std::string & separator = ", ") const;
		std::vector<glm::vec3> loadCoRsFromBinaryFile(const std::string & path) const;
	};

	class BFSCoRCalculator : public CoRCalculator {

		float _bfsEpsilon = 0.000001f;

		void calculateANNData(CoRMesh * mesh, float omega) const;

	protected:
		void convertTriangles(std::vector<unsigned int> triangleIndices, CoRMesh *mesh) const override;

	public:
		explicit BFSCoRCalculator(
				float sigma = 0.1f,
				float omega = 0.1f,
				bool subdivide = true,
				unsigned int numberOfThreadsToCreate = 4,
				float bfsEpsilon = 0.000001f);

		bool calculateCoR(unsigned long vertex, const CoRMesh &mesh, glm::vec3* corOut) const override;
	};
}

#endif // COR_CORCALCULATOR_H