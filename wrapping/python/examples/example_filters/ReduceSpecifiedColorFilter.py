from typing import List
import simplnx as nx

class ReduceSpecifiedColorFilter:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  IMAGE_PATH_KEY = 'image_path'
  COLOR_TO_REDUCE_KEY = 'color_to_reduce'
  REDUCTION_PERCENTAGE_KEY = 'reduction_percentage'
  OUTPUT_PATH_KEY = 'output_path'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('487984f8-3a78-4d12-aff4-4feb148c715e')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'ReduceSpecifiedColorFilter'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'ReduceSpecifiedColorFilter'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'ReduceSpecifiedColorFilter'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  ReduceSpecifiedColorFilter
    """
    return ReduceSpecifiedColorFilter()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(ReduceSpecifiedColorFilter.IMAGE_PATH_KEY, 'Input File', '', '', {'.png', '.jpg.', 'tif'}, nx.FileSystemPathParameter.PathType.InputFile, False))
    params.insert(nx.ChoicesParameter(ReduceSpecifiedColorFilter.COLOR_TO_REDUCE_KEY, 'Color', 'Choose which color to reduce', 0, ['red', 'green', 'blue']))
    params.insert(nx.Int32Parameter(ReduceSpecifiedColorFilter.REDUCTION_PERCENTAGE_KEY, 'Reduction %', 'Percentage Intensity that the color chosen is reduced by', 50))
    params.insert(nx.FileSystemPathParameter(ReduceSpecifiedColorFilter.OUTPUT_PATH_KEY, 'Output File', '', '', {'.png', '.jpg.', 'tif'}, nx.FileSystemPathParameter.PathType.OutputFile, False))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight Complete'))
    return nx.IFilter.PreflightResult()

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """

    from PIL import Image

    # Variables to be defined at the top
    image_path = args[ReduceSpecifiedColorFilter.IMAGE_PATH_KEY]
    color_to_reduce: int = args[ReduceSpecifiedColorFilter.COLOR_TO_REDUCE_KEY]
    reduction_percentage: int = args[ReduceSpecifiedColorFilter.REDUCTION_PERCENTAGE_KEY]
    output_path = args[ReduceSpecifiedColorFilter.OUTPUT_PATH_KEY]

    # Load the image
    image = Image.open(image_path)
    pixels = image.load()

    # Calculate the reduction factor based on the specified percentage
    reduction_factor = (100 - reduction_percentage) / 100

    # Process the image to adjust the specified color channel by the reduction factor
    for i in range(image.width):
        for j in range(image.height):
            r, g, b = pixels[i, j]

            if color_to_reduce == 0:
                r = int(r * reduction_factor)
            elif color_to_reduce == 1:
                g = int(g * reduction_factor)
            elif color_to_reduce == 2:
                b = int(b * reduction_factor)

            pixels[i, j] = (r, g, b)

    # Display or save the modified image
    # image.show()  # Or use 
    image.save(output_path)

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute Complete'))
    return nx.Result()

