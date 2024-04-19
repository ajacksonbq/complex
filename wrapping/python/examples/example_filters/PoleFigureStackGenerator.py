from typing import List
import simplnx as nx

import itkimageprocessing as nxitk
import orientationanalysis as nxor

import numpy as np

import os
import shutil
from pathlib import Path

#------------------------------------------------------------------------------------
#---                               Check Filter                                   ---
#------------------------------------------------------------------------------------
def check_filter_result(filter: nx.IFilter, result: nx.IFilter.ExecuteResult) -> None:
  if len(result.warnings) != 0:
    print(f'{filter.name()} ::  Warnings: {result.warnings}')
  
  has_errors = len(result.errors) != 0 
  if has_errors:
    print(f'{filter.name()} :: Errors: {result.errors}')
    raise RuntimeError(result)
  
  print(f"{filter.name()} :: No errors running the filter")

class PoleFigureStackGenerator:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  LIST_PARAMETER_KEY = 'list_parameter'
  INPUT_DIR_KEY = 'input_directory'
  OUTPUT_DIR_KEY = 'output_directory'
  FILE_PREFIX_PARAMETER_KEY = 'file_prefix'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('d883d38a-0a7b-41b3-a784-2bcbabcf37c6')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'PoleFigureStackGenerator'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'PoleFigureStackGenerator'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'PoleFigureStackGenerator'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  PoleFigureStackGenerator
    """
    return PoleFigureStackGenerator()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    generated_file_list_value = nx.GeneratedFileListParameter.ValueType()
    # generated_file_list_value.input_path = "DREAM3DNXData/Data/Porosity_Image"
    generated_file_list_value.ordering = nx.GeneratedFileListParameter.Ordering.LowToHigh
    generated_file_list_value.file_prefix = "slice-"
    generated_file_list_value.file_suffix = ""
    generated_file_list_value.file_extension = ".ctf"
    generated_file_list_value.start_index = 11
    generated_file_list_value.end_index = 174
    generated_file_list_value.increment_index = 1
    generated_file_list_value.padding_digits = 2

    params.insert(nx.FileSystemPathParameter(PoleFigureStackGenerator.OUTPUT_DIR_KEY, 'Output Dir', '', '', {'.*'}, nx.FileSystemPathParameter.PathType.OutputDir, False))
    params.insert(nx.StringParameter(PoleFigureStackGenerator.FILE_PREFIX_PARAMETER_KEY, 'File Prefix', '', ''))
    params.insert(nx.GeneratedFileListParameter(PoleFigureStackGenerator.LIST_PARAMETER_KEY, 'List', '', generated_file_list_value))


    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight Success'))
    return nx.IFilter.PreflightResult()

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """

    files: nx.GeneratedFileListParameter.ValueType = args[PoleFigureStackGenerator.LIST_PARAMETER_KEY]
    prefix = args[PoleFigureStackGenerator.FILE_PREFIX_PARAMETER_KEY]
    output_dir = args[PoleFigureStackGenerator.OUTPUT_DIR_KEY]
    file_list = files.generate()
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'{files}'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'{file_list}'))

    for file in file_list:
      self.PoleFigureStackGeneratorFunction(prefix=prefix, input_file=file, output_dir=output_dir)

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute Complete'))
    return nx.Result()
  
  #------------------------------------------------------------------------------
  # Function##
  #------------------------------------------------------------------------------
  def PoleFigureStackGeneratorFunction(self, prefix, input_file, output_dir):
    
    #------------------------------------------------------------------------------
    # Create a Data Structure
    #------------------------------------------------------------------------------
    data_structure = nx.DataStructure()

    # Import a CTF Data file
    result = nxor.ReadCtfDataFilter.execute(data_structure=data_structure, 
                                  cell_attribute_matrix_name="CellData", 
                                  cell_ensemble_attribute_matrix_name="Phase Data",
                                  data_container_name=nx.DataPath(["DataContainer"]), 
                                  input_file=input_file)

    check_filter_result(nxor.ReadCtfDataFilter, result)

    #------------------------------------------------------------------------------
    # Rotate the Sample Reference Frame 180@010
    #------------------------------------------------------------------------------
    nx.RotateSampleRefFrameFilter.execute(data_structure=data_structure,
        # created_image_geometry = nx.DataPath("Foo"),
        remove_original_geometry=True,
        rotate_slice_by_slice=False,
        rotation_axis=[0.0,1.0,0.0,180.0],
        rotation_representation=0,
        # rotation_matrix=[[1,0,0],[0,1,0],[0,0,1]],
        selected_image_geometry=nx.DataPath(["DataContainer"])
        )
    check_filter_result(nx.RotateSampleRefFrameFilter, result)


    #------------------------------------------------------------------------------
    # Create a ThresholdSet to use in the MultiThreshold Objects filter
    # This will create a boolean output array at DataPath(["DataContainer", "CellData", "Mask"]
    #------------------------------------------------------------------------------
    threshold_1 = nx.ArrayThreshold()
    threshold_1.array_path = nx.DataPath(["DataContainer", "CellData", "Error"])
    threshold_1.comparison = nx.ArrayThreshold.ComparisonType.Equal
    threshold_1.value = 0

    threshold_set = nx.ArrayThresholdSet()
    threshold_set.thresholds = [threshold_1]
    dt = nx.DataType.boolean
    result = nx.MultiThresholdObjects.execute(data_structure=data_structure,
                                            array_thresholds=threshold_set, 
                                            created_data_path="Mask",
                                            created_mask_type=nx.DataType.boolean)
    check_filter_result(nx.MultiThresholdObjects, result)


    #------------------------------------------------------------------------------
    # Generate the IPF Colors for the <001> direction
    #------------------------------------------------------------------------------
    result = nxor.GenerateIPFColorsFilter.execute(    data_structure=data_structure,
        cell_euler_angles_array_path=nx.DataPath(["DataContainer", "CellData", "EulerAngles"]),
        cell_ipf_colors_array_name="IPFColors",
        cell_phases_array_path=nx.DataPath(["DataContainer", "CellData", "Phases"]),
        crystal_structures_array_path=nx.DataPath(["DataContainer", "Phase Data", "CrystalStructures"]),
        mask_array_path=nx.DataPath(["DataContainer", "CellData", "Mask"]),
        reference_dir=[0,0,1],
        use_mask=True
    )
    check_filter_result(nxor.GenerateIPFColorsFilter, result)



    #------------------------------------------------------------------------------
    # Write the IPF colors to a PNG file
    #------------------------------------------------------------------------------
    input_file = Path(input_file)
    file_output_name = output_dir + input_file.name + "_IPF_Z.png"
    result = nxitk.ITKImageWriter.execute(data_structure=data_structure, 
                                          file_name=file_output_name,
                                          image_array_path=nx.DataPath(["DataContainer", "CellData", "IPFColors"]),
                                          image_geom_path=nx.DataPath(["DataContainer"]),
                                          index_offset=0,
                                          plane=0)
    check_filter_result(nxitk.ITKImageWriter, result)

    #------------------------------------------------------------------------------
    # Create a Pole Figure
    #------------------------------------------------------------------------------
    file_output_name = output_dir + input_file.name + "_PoleFigure.png"
    result = nxor.WritePoleFigureFilter.execute(data_structure=data_structure,
                                                cell_euler_angles_array_path=nx.DataPath(["DataContainer", "CellData", "EulerAngles"]), 
                                                cell_phases_array_path=nx.DataPath(["DataContainer", "CellData", "Phases"]),
                                                crystal_structures_array_path=nx.DataPath(["DataContainer", "Phase Data", "CrystalStructures"]),
                                                generation_algorithm=1, # Discrete = 1 
                                                mask_array_path=nx.DataPath(["DataContainer", "CellData", "Mask"]), 
                                                image_geometry_path=nx.DataPath(["DataContainer Pole Figure"]), 
                                                image_layout=0, # O = Horizontal Layout 
                                                image_prefix=input_file.name, 
                                                image_size=512, 
                                                lambert_size=64, 
                                                material_name_array_path=nx.DataPath(["DataContainer", "Phase Data", "MaterialName"]), 
                                                num_colors=32, 
                                                output_path=output_dir,
                                                save_as_image_geometry=True, 
                                                title= input_file.name + " Pole Figure",
                                                use_mask=True, 
                                                write_image_to_disk=True)
    check_filter_result(nxor.WritePoleFigureFilter, result)



    #------------------------------------------------------------------------------
    # Write the DataStructure to a .dream3d file
    #------------------------------------------------------------------------------

    # output_file_path = check_filter_result() / "basic_ebsd_example.dream3d"
    # result = nx.WriteDREAM3DFilter.execute(data_structure=data_structure, 
    #                                         export_file_path=output_dir, 
    #                                         write_xdmf_file=True)
    # check_filter_result(nx.WriteDREAM3DFilter, result)
