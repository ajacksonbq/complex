# K Means

## Group (Subgroup)

DREAM3D Review (Clustering)

## Description

***Warning:* The randomnes in this filter is not currently consistent between operating systems even if the same seed is used. Specifically between Unix and Windows. This does not affect the results, but the IDs will not correspond. For example if the Cluster Identifier at index one on Linux is 1 it could be 2 on Windows, the overarching clusters will be the same, but their IDs will be different.**

This **Filter** applies the k means algorithm to an **Attribute Array**.  K means is a *clustering algorithm* that assigns to each point of the **Attribute Array** a *cluster Id*.  The user must specify the number of clusters in which to partition the array.  Specifically, a k means partitioning is a *Voronoi tesselation*; an optimal solution to the k means problem is such that each point in the data set is associated with the cluster that has the closest mean.  This partitioning is the one that minimizes the within cluster variance (i.e., minimizes the within cluster sum of squares differences).  Thus, the "metric" used for k means is the 2-norm (the *Euclidean norm*; the squared Euclidean norm may also be used since this maintains the triangle inequality).

Optimal solutions to the k means partitioning problem are computationally difficult; this **Filter** used *Lloyd's algorithm* to approximate the solution.  Lloyd's algorithm is an iterative algorithm that proceeds as follows:

1. Choose k points at random to serve as the initial cluster "means"
2. Until convergence, repeat the following steps:

- Associate each point with the closest mean, where "closest" is the smallest 2-norm distance
- Recompute the means based on the new tesselation

Convergence is defined as when the computed means change very little (precisely, when the differences are within machine epsilon).  Since Lloyd's algorithm is iterative, it only serves as an approximation, and may result in different classifications on each execution with the same input data.  The user may opt to use a mask to ignore certain points; where the mask is *false*, the points will be placed in cluster 0.

A clustering algorithm can be considered a kind of segmentation; this implementation of k means does not rely on the **Geometry** on which the data lie, only the *topology* of the space that the array itself forms.  Therefore, this **Filter** has the effect of creating either **Features** or **Ensembles** depending on the kind of array passed to it for clustering.  If an **Element** array (e.g., voxel-level **Cell** data) is passed to the **Filter**, then **Features** are created (in the previous example, a **Cell Feature Attribute Matrix** will be created).  If a **Feature** array is passed to the **Filter**, then an Ensemble Attribute Matrix** is created.  The following table shows what type of **Attribute Matrix** is created based on what sort of array is used for clustering:

| Attribute Matrix Source             | Attribute Matrix Created |
|------------------|--------------------|
| Generic | Generic |
| Vertex | Vertex Feature |
| Edge | Edge Feature |
| Face | Face Feature |
| Cell | Cell Feature|
| Vertex Feature | Vertex Ensemble |
| Edge Feature | Edge Ensemble |
| Face Feature | Face Ensemble |
| Cell Feature | Cell Ensemble|
| Vertex Ensemble | Vertex Ensemble |
| Edge Ensemble | Edge Ensemble |
| Face Ensemble | Face Ensemble |
| Cell Ensemble | Cell Ensemble|

This **Filter** will store the means for the final clusters within the created **Attribute Matrix**.

% Auto generated parameter table will be inserted here

## References

[1] Least squares quantization in PCM, S.P. Lloyd, IEEE Transactions on Information Theory, vol. 28 (2), pp. 129-137, 1982.

## Example Pipelines

## License & Copyright

Please see the description file distributed with this plugin.

## DREAM3D-NX Help

If you need help, need to file a bug report or want to request a new feature, please head over to the [DREAM3DNX-Issues](https://github.com/BlueQuartzSoftware/DREAM3DNX-Issues) GItHub site where the community of DREAM3D-NX users can help answer your questions.
