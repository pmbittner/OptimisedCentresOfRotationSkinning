#include <iostream>
#include <glm/vec3.hpp>

#include <cor/CoRCalculator.h>
#include <cor/CoRMesh.h>

int main(int argc, char** argv) {
    using namespace CoR;

    std::vector<glm::vec3> vertices; // get some vertices here
    std::vector<unsigned int> faces; // Triangle data: Three consecutive values describe a triangle.
    std::vector<std::vector<unsigned int>> boneIndices; // bone indices for each vertex
    std::vector<std::vector<float>> boneWeights; // weights for each vertex
    unsigned int numberOfBones;

    // Choose computation values
    float subdivEpsilon = 0.1f;
    float sigma = 0.1f;
    float omega = 0.1f;
    float bfsEpsilon = 0.000001f;
    unsigned int numberOfThreadsToUse = 8;
    bool performSubdivision = true;

    // Choose one
    CoRCalculator c(sigma, omega, performSubdivision, numberOfThreadsToUse);
    //BFSCoRCalculator cs(sigma, omega, performSubdivision, numberOfThreadsToUse, bfsEpsilon);

    // Create Mesh of CoRCalculation API with conversion methods
    std::vector<WeightsPerBone> weightsPerBone = c.convertWeights(numberOfBones, boneIndices, boneWeights);
    CoRMesh mesh = c.createCoRMesh(vertices, faces, weightsPerBone, subdivEpsilon);

    // Start asynchronously computation. In the callback we just write the cors to a file.
    c.calculateCoRsAsync(mesh, [&c](std::vector<glm::vec3> &cors) {
        c.saveCoRsToBinaryFile("/path/to/my/output/file.cors", cors);
        c.saveCoRsToTextFile("/path/to/my/debug/inspection/file.txt", cors);
    });

    // Loading can be performed from binary files only so far.
    std::vector<glm::vec3> cors = c.loadCoRsFromBinaryFile("/path/to/another/output/file.cors");
}