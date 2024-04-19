from typing import List
import simplnx as nx
import pandas as pd
import numpy as np

class PandasTesting:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  SELECTED_ARRAYS_KEY = 'selected_arrays_path'
  FILE_OUTPUT_KEY = 'file_output_path'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('49fd435e-a1e8-403a-a926-3edd1fb6c2a8')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'PandasTesting'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'PandasTesting'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'PandasTesting'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  PandasTesting
    """
    return PandasTesting()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.MultiArraySelectionParameter(PandasTesting.SELECTED_ARRAYS_KEY, 'MultiArraySelectionParameter', 'Example multiarray selection help text', [], set([nx.IArray.ArrayType.DataArray]), nx.get_all_data_types(), [[1]]))
    params.insert(nx.FileSystemPathParameter(PandasTesting.FILE_OUTPUT_KEY, 'Output Directory', '', '', {'.xlsx'}, nx.FileSystemPathParameter.PathType.OutputFile))

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

    input_data_paths = args[PandasTesting.SELECTED_ARRAYS_KEY]
    excel_file_path = args[PandasTesting.FILE_OUTPUT_KEY]
    
    # Create a Dictioary of Data Array names to numpy views of the Data Array
    # This will be used later to populate the data frame
    array_dict = {}

    # Loop over each of the user selected nx.DataPaths
    for data_path in input_data_paths:
        npview = data_structure[data_path].npview()     # Get a numpy view of the Data Array
        # Squeeze the Numpy View to remove the component dimensions
        # All Selected Arrays are Guarenteed to be Single Component because the parameter only allows single component
        array_dict[data_path.name()] = np.squeeze(npview)

    # Create Data Frame from dictionary 
    df = pd.DataFrame.from_dict(array_dict)

    # Write out to Excel with Default Sheet Name
    df.to_excel(excel_file_path, sheet_name='Sheet 1', index=False)

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute Complete: File Created at {excel_file_path}'))
    return nx.Result()