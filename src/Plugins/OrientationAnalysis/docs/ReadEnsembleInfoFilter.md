# Import Ensemble Info File

## Group (Subgroup)

IO (Input)

## Description

This **Filter** reads in information about the crystal structure and phase types of all the **Features** that are contained in a **Cell** based volume. These values are needed to allow the calculation of statistics on the volume, if they have not already been provided by some other means.  The format of the input file is a simple ASCII text file with the extension .ini or .txt. The first group in the file is the name [EnsembleInfo] in square brackets with the key Number_Phases=*number of phases* that are contained in the volume. Subsequent groups in the file list the **Phase Number**, **Crystal Structure** and **Phase Type**. The proper values for the crystal structure and phase type come from internal constants within DREAM.3D and are listed here:

**Crystal Structure**

| String Name | Internal Value | Laue Name |
| ------------|----------------|----------|
| Hexagonal_High | 0 |  Hexagonal-High 6/mmm |
| Cubic_High | 1 |  Cubic Cubic-High m3m |
| Hexagonal_Low | 2 |  Hexagonal-Low 6/m |
| Cubic_Low | 3 |  Cubic Cubic-Low m3 (Tetrahedral) |
| Triclinic | 4 |  Triclinic -1 |
| Monoclinic | 5 |  Monoclinic 2/m |
| OrthoRhombic | 6 |  Orthorhombic mmm |
| Tetragonal_Low | 7 |  Tetragonal-Low 4/m |
| Tetragonal_High | 8 |  Tetragonal-High 4/mmm |
| Trigonal_Low | 9 |  Trigonal-Low -3 |
| Trigonal_High | 10 |  Trigonal-High -3m |
| UnknownCrystalStructure | 999 |  Undefined Crystal Structure |

**Phase Type**

| String Name | Internal Value |
| ------------|----------------|
| PrimaryPhase | 0 |
| PrecipitatePhase | 1 |
| TransformationPhase | 2 |
| MatrixPhase | 3 |
| BoundaryPhase | 4 |
| UnknownPhaseType | 999 |

## Example Input

**Phase numbering starts at One (1). Phase Zero (0) is reserved for internal use in DREAM.3D**
For example, if you have a structure that has 2 phases that consist of a Cubic Primary phase and a Hexagonal Matrix phase the file would be the following:

    [EnsembleInfo]
    Number_Phases=2

    [1]
    CrystalStructure=Cubic_High
    PhaseType=PrimaryPhase

    [2]
    CrystalStructure=Hexagonal_High
    PhaseType=MatrixPhase

% Auto generated parameter table will be inserted here

## Example Pipelines

## License & Copyright

Please see the description file distributed with this **Plugin**

## DREAM3D-NX Help

If you need help, need to file a bug report or want to request a new feature, please head over to the [DREAM3DNX-Issues](https://github.com/BlueQuartzSoftware/DREAM3DNX-Issues) GItHub site where the community of DREAM3D-NX users can help answer your questions.
