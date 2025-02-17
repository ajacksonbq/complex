# Find Feature Shapes

## Group (Subgroup)

Statistics (Morphological)

## Description

This **Filter** calculates the second-order moments of each **Feature** in order to determine the *principal axis lengths, principal axis directions, aspect ratios and moment invariant Omega3s*.  The *principal axis lengths* are those of a "best-fit" ellipsoid.  The algorithm for determining the moments and these values is as follows:

1. For each **Cell**, determine the x, y and z distance to the centroid of the **Feature** that owns the **Cell**
2. For each **Cell**, calculate Ixx, Iyy, Izz, Ixy, Ixz and Iyz using the x, y and z distances determined in step 1.
3. Sum the individual Ixx, Iyy, Izz, Ixy, Ixz and Iyz values for all **Cells** belonging to the same **Feature**
4. Find the *eigenvalues* and *eigenvectors* of the *3x3* symmetric matrix defined by the *6* values calculated in step 3 for each **Feature**
5. Use the relationship of *principal moments* to the *principal axis lengths* for an ellipsoid, which can be found in [4], to determine the *Semi-Axis Lengths*
6. Calculate the *Aspect Ratios* from the *Semi-Axis Lengths* found in step 5.
7. Determine the Euler angles required to represent the *principal axis directions* in the *sample reference frame* and store them as the **Feature**'s *Axis Euler Angles*.
8. Calculate the moment variant Omega3 as definied in [2] and is discussed further in [1] and [3]

% Auto generated parameter table will be inserted here

## References ##

[1] Representation and Reconstruction of Three-dimensional Microstructures in Ni-based Superalloys, AFOSR FA9550-07-1-0179 Final Report, 20 Dec 2010.

[2] On the use of moment invariants for the automated classifcation of 3-D particle shapes, J. MacSleyne, J.P. Simmons and M. De Graef, Modeling and Simulations in Materials Science and Engineering, 16, 045008 (2008).

[3] n-Dimensional Moment Invariants and Conceptual Mathematical Theory of Recognition n-Dimensional Solids, Alexander G. Mamistvalov, IEEE TRANSACTIONS ON PATTERN ANALYSIS AND MACHINE INTELLIGENCE, VOL. 20, NO. 8, AUGUST 1998, p. 819-831.

[4] M. Groeber, M. Uchic, D. Dimiduk, and S. Ghosh.    A Framework for Automated Analysis and Simulation of 3D Polycrystalline Microstructures, Part 1: Statistical Characterization    Acta Materialia, 56 (2008), 1257-1273.

## Example Pipelines

+ (01) SmallIN100 Morphological Statistics
+ (06) SmallIN100 Synthetic

## License & Copyright

Please see the description file distributed with this **Plugin**

## DREAM3D-NX Help

If you need help, need to file a bug report or want to request a new feature, please head over to the [DREAM3DNX-Issues](https://github.com/BlueQuartzSoftware/DREAM3DNX-Issues) GItHub site where the community of DREAM3D-NX users can help answer your questions.
