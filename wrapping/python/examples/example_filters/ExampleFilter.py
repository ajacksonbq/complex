
from typing import List
import simplnx as nx

class ExampleFilter:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  TEST_KEY = 'test'
  TEST_KEY_1 = 'test1'
  TEST_KEY_2 = 'test2'
  TEST_KEY_3 = 'test3'  
  TEST_KEY_4 = 'test4'
  TEST_KEY_5 = 'test5'
  TEST_KEY_60 = 'test60'
  TEST_KEY_7 = 'test7'
  FILE_PATH = 'file1'
  STRING_PARAMETER = 'str1'
  INTEGER_PARAMETER = 'int1'
  STRING_2_PARAMETER = 'str2'


  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('55e6b725-16a0-4777-946e-61f97444ef71')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'ExampleFilter'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'ExampleFilter'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'ExampleFilter'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  ExampleFilter
    """
    return ExampleFilter()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.Float64Parameter(ExampleFilter.TEST_KEY, 'Test', 'boom', 0.0))
    params.insert(nx.Float64Parameter(ExampleFilter.TEST_KEY_1, 'Screenshot #', '', 420.69))
    params.insert(nx.Float64Parameter(ExampleFilter.TEST_KEY_2, 'Test2', '', 300.69))
    params.insert(nx.Float64Parameter(ExampleFilter.TEST_KEY_3, 'Test3', '', 0.0))
    params.insert(nx.Float64Parameter(ExampleFilter.TEST_KEY_4, 'Test4', '', 190))
    params.insert(nx.FileSystemPathParameter(ExampleFilter.TEST_KEY_5, 'Screenshot Path', "Where you want the thing to go", '',{'.png', '.jpeg'}, nx.FileSystemPathParameter.PathType.InputFile))
    params.insert(nx.StringParameter(ExampleFilter.TEST_KEY_60, 'Test String', 'String', 'some string'))
    params.insert(nx.BoolParameter(ExampleFilter.TEST_KEY_7, 'Test Bool', '', True))
    # params.insert(nx.ChoicesParameter('arg0', 'arg1', 'arg2', 2, ['l1', 'l2', 'l3', 'l4']))
    # params.insert(nx.Int8Parameter('arg6', 'arg7', 'arg8', 3 ))
    # params.insert(nx.StringParameter(ExampleFilter.TEST_KEY_60, 'Test String', 'String', 'some string'))

    params.insert(nx.FileSystemPathParameter(ExampleFilter.FILE_PATH, 'Image Path', '.png Image Path to Increase Pixels', '', {'.png'}, nx.FileSystemPathParameter.PathType.InputFile))
    params.insert(nx.StringParameter(ExampleFilter.STRING_PARAMETER, 'Background Color', 'Enter a String Color', 'black'))
    params.insert(nx.Int64Parameter(ExampleFilter.INTEGER_PARAMETER, 'Pixel Increase Amount', 'Enter The Amount of Pixels To Increase Your Image By', 200))
    params.insert(nx.StringParameter(ExampleFilter.STRING_2_PARAMETER, 'New Name (Optional)', 'Enter a New Name For Your Edited Image (If Left Empty, Name Will Not Be Changed)', ''))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """
    value: float = args[ExampleFilter.TEST_KEY]
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight: {value}'))
    return nx.IFilter.PreflightResult()

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """

    value: float = args[ExampleFilter.TEST_KEY]
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute: {value}'))
    return nx.Result()


