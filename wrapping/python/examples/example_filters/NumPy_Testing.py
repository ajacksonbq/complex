
from typing import List
import simplnx as nx
import numpy as np

class NumPy_Testing:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  OPERATION = 'arithmetic_operation'
  ADD = 'addition'
  SUBTRACT = 'subtraction'
  MULTIPLY = 'multiplication'
  DIVIDE = 'division'
  SQRT = 'square_root'
  EXPONENT = 'power'
  NUMPY_CHOICES_LIST = [ADD, SUBTRACT, MULTIPLY, DIVIDE, SQRT, EXPONENT]
  INPUT_ARRAY_PATH = 'input_array'
  INPUT_ARRAY_PATH_2 = 'input_array_2'
  OUTPUT_ARRAY_KEY = 'output_array'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('095e6186-b9f6-493c-8a90-3ac4beb09406')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'NumPy_Testing'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'NumPy_Testing'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'NumPy_Testing'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  NumPy_Testing
    """
    return NumPy_Testing()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.Parameters.Separator("Parameters"))
    params.insert(nx.ChoicesParameter(NumPy_Testing.OPERATION, 'NumPy Operations', '', 0, NumPy_Testing.NUMPY_CHOICES_LIST))
    params.insert(nx.Parameters.Separator("Required Data Objects"))
    params.insert(nx.ArraySelectionParameter(NumPy_Testing.INPUT_ARRAY_PATH, 'Input Array', '', nx.DataPath(), {nx.DataType.float32, nx.DataType.float64}, [[1]]))
    params.insert(nx.ArraySelectionParameter(NumPy_Testing.INPUT_ARRAY_PATH_2, 'Input Array', '', nx.DataPath(), {nx.DataType.float32, nx.DataType.float64}, [[1]]))
    params.insert(nx.Parameters.Separator("Created Data Objects"))
    # params.insert(nx.ArrayCreationParameter(NumPy_Testing.OUTPUT_ARRAY_KEY, 'Output Array', '', nx.DataPath([])))
    params.insert(nx.DataObjectNameParameter(NumPy_Testing.OUTPUT_ARRAY_KEY, "Output Array Name", '', "Output Array"))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """

    input_array_paths: nx.DataPath = args[NumPy_Testing.INPUT_ARRAY_PATH]
    input_array_paths_2: nx.DataPath = args[NumPy_Testing.INPUT_ARRAY_PATH_2]
    # output_array_path: nx.DataPath = args[NumPy_Testing.OUTPUT_ARRAY_KEY]
    output_array_name: str = args[NumPy_Testing.OUTPUT_ARRAY_KEY]

    errors = []
    warnings = []
    output_actions = nx.OutputActions()

    input_array: nx.IDataArray = data_structure[input_array_paths]

    operation = args[NumPy_Testing.OPERATION]

    if operation == 0:
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Operation: Addition '))
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Choice Verification: ({operation})'))

    elif operation == 1:
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Operation: Subtraction '))
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Choice Verification: ({operation})'))

    elif operation == 2:
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Operation: Multiplication '))
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Choice Verification: ({operation})'))

    elif operation == 3:
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Operation: Division '))
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Choice Verification: ({operation})'))

    elif operation == 4:
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Operation: Square Root '))
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Choice Verification: ({operation})'))

    elif operation == 5:
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Operation: Power '))
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Choice Verification: ({operation})'))

    output_array_path = nx.DataPath(['Geometry', 'Cell Data', output_array_name])
    output_actions.append_action(nx.CreateArrayAction(nx.DataType.float32, input_array.tdims, input_array.cdims, output_array_path))
    return nx.IFilter.PreflightResult(output_actions, errors=None, warnings=warnings)

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """

    operation = args[NumPy_Testing.OPERATION]

    input_array_paths: nx.DataPath = args[NumPy_Testing.INPUT_ARRAY_PATH]
    input_array = data_structure[input_array_paths].npview()

    input_array_paths_2: nx.DataPath = args[NumPy_Testing.INPUT_ARRAY_PATH_2]
    input_array_2 = data_structure[input_array_paths_2].npview()

    output_array_name: str = args[NumPy_Testing.OUTPUT_ARRAY_KEY]

    output_array_path = nx.DataPath(['Geometry', 'Cell Data', output_array_name])
    output_array = data_structure[output_array_path].npview()


    if operation == 0:
      result = input_array + input_array_2

    elif operation == 1:
      result = input_array - input_array_2

    elif operation == 2:
      result = input_array * input_array_2

    elif operation == 3:
      result = input_array / input_array_2

    elif operation == 4:
      result = np.sqrt(input_array)

    elif operation == 5:
      result = np.power(input_array, input_array_2)

    output_array[:] = result
    return nx.Result()