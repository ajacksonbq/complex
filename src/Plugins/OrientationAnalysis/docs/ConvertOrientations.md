# Convert Orientation Representations

## Group (Subgroup)

Orientation Analysis (Conversion)

## Description

This **Filter** generates a new orientation representation (see Data Layout Table below) for each **Element**, given the *Input Orientation Representation* for the **Element**. The following table lists the various orientation representations that are supported. DREAM3D is capable of converting between any representation with some caveats.

### Data Layout

| Orientation Representation | No. of Components | Representation Type | Data Layout |
|----------------------------|----------|---------------------|-------------|
| EulerAngles                | 3 | 0 | phi1, Phi, phi2 |
| Orientation Matrix         | 9 | 1 | Row Major Format |
| Quaternions                | 4 | 2 | ( \[x, y, z\], w ) |
| Axis Angle                 | 4 | 3 | ( \[x, y, z\], Angle) |
| Rodrigues Vectors          | 4 | 4 | *Internally DREAM.3D keeps a 4th component* |
| Homochoric                 | 3 | 5 | \[x, y, z\] |
| Cubochoric                 | 3 | 6 | \[x, y, z\] |
| StereoGraphic              | 3 | 7 | \[x, y, z\] |

### Data Range

The valid range for Euler angles is (Degrees):

    + phi-1: 0 to 360
    + Phi : 0 to 180
    + phi-2: 0 to 360

### Data Conversion Notes

If the angles fall outside of this range the **original** Euler Input data **WILL BE CHANGED** to ensure they are within this range.

## Precision Notes

While every effort has been made to ensure the correctness of each transformation algorithm, certain situations may arise where the initial precision of the input data is not large enough for the algorithm to calculate an answer that is intuitive. The user should be acutely aware of their input data and if their data may cause these situations to occur. Combinations of Euler angles close to 0, 180 and 360 can cause these issues to be hit. For instance an Euler angle of [180, 56, 360] is symmetrically the same as [180, 56, 0] and due to calculation errors and round off errors converting that Euler angle between representations may not give the numerical answer the user was anticipating but will give a symmetrically equivalent angle.

% Auto generated parameter table will be inserted here

## Example Pipelines

+ 02_Adaptive Alignment - Misorientation - Zero Shifts
+ (10) SmallIN100 Full Reconstruction
+ INL Export
+ 03_Adaptive Alignment - Mutual Information - SEM Images
+ MassifPipeline
+ InsertTransformationPhase
+ 04_Steiner Compact
+ (03) SmallIN100 Alignment

## License & Copyright

Please see the description file distributed with this **Plugin**

## DREAM3D-NX Help

If you need help, need to file a bug report or want to request a new feature, please head over to the [DREAM3DNX-Issues](https://github.com/BlueQuartzSoftware/DREAM3DNX-Issues) GItHub site where the community of DREAM3D-NX users can help answer your questions.
