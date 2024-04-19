
#Background Color

from typing import List
import simplnx as nx
from PIL import Image

class Example_Filter_2:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  FILE_PATH = 'file1'
  BACKGROUND_COLOR = 'str1'
  INCREASE_AMOUNT = 'int1'
  NEW_NAME = 'str2'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('d6717111-a5c5-4d13-8a35-91bd08672baa')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'Example_Filter_2'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'Example_Filter_2'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'Example_Filter_2'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  Example_Filter_2
    """
    return Example_Filter_2()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(Example_Filter_2.FILE_PATH, 'Image Path', 'Original Image Path', '', {'.png'}, nx.FileSystemPathParameter.PathType.InputFile))
    params.insert(nx.StringParameter(Example_Filter_2.BACKGROUND_COLOR, 'Background Color', 'Enter a String Color', 'black'))
    params.insert(nx.Int16Parameter(Example_Filter_2.INCREASE_AMOUNT, 'Pixel Increase Amount', 'Enter The Amount of Pixels To Increase Your Image By (Decrease with Negative Number (-200))', 200))
    params.insert(nx.FileSystemPathParameter(Example_Filter_2.NEW_NAME, 'Output File Path', 'Created Image Path', '', {'.png'}, nx.FileSystemPathParameter.PathType.OutputFile))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """

    pixel_increase: int = args[Example_Filter_2.INCREASE_AMOUNT]
    new_pixels_color: str = args[Example_Filter_2.BACKGROUND_COLOR]
    new_file_name: str = args[Example_Filter_2.NEW_NAME]
    image_path: str = args[Example_Filter_2.FILE_PATH]
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight: {image_path}'))
    return nx.IFilter.PreflightResult()


  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """
    pixel_increase: int = args[Example_Filter_2.INCREASE_AMOUNT]
    new_file_name: str = args[Example_Filter_2.NEW_NAME]
    new_pixels_color: str = args[Example_Filter_2.BACKGROUND_COLOR]
    file_path: str = args[Example_Filter_2.FILE_PATH]
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight: {file_path}'))

    # Half of the pixel_increase should be applied to each side
    half_pixel_increase = pixel_increase // 2

    # Open the original image
    original_image = Image.open(file_path)

    # Calculate the new size
    new_width = original_image.width + pixel_increase
    new_height = original_image.height + pixel_increase

    # Create a new image with the new size and fill it with the new_pixels_color
    new_image = Image.new("RGB", (new_width, new_height), new_pixels_color)

    # Paste the original image onto the center of the new image
    new_image.paste(original_image, (half_pixel_increase, half_pixel_increase))

    # Determine the save path
    save_path = new_file_name if new_file_name else file_path

    # Save the modified image
    new_image.save(save_path)

    return nx.Result()
