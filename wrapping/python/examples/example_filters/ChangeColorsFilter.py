from typing import List
import simplnx as nx
from PIL import Image, ImageEnhance

class ChangeColorsFilter:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  INPUT_IMAGE_PATH_KEY = 'input_image_path'
  OUTPUT_IMAGE_PATH_KEY = 'output_image_path'
  HUE_SHIFT_DEGREES_KEY = 'hue_shift_degrees'
  SATURATION_PERCENTAGE_KEY = 'saturation_percentage'
  VALUE_PERCENTAGE_KEY = 'value_percentage'
  BRIGHTNESS_PERCENTAGE_KEY = 'brightness_percentage'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('5b7900a4-7458-4b31-a644-f792ded3d227')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'ChangeColorsFilter'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'ChangeColorsFilter'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'ChangeColorsFilter'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  ChangeColorsFilter
    """
    return ChangeColorsFilter()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(ChangeColorsFilter.INPUT_IMAGE_PATH_KEY, 'Input File', '', '', {'.png', '.jpg.', 'tif'}, nx.FileSystemPathParameter.PathType.InputFile, False))
    params.insert(nx.Int32Parameter(ChangeColorsFilter.HUE_SHIFT_DEGREES_KEY, 'Hue', '-360 - 360', 0))
    params.insert(nx.Int32Parameter(ChangeColorsFilter.SATURATION_PERCENTAGE_KEY, 'Saturation', '< 100 = Decrease, > 100 = Increase', 100))
    params.insert(nx.Int32Parameter(ChangeColorsFilter.VALUE_PERCENTAGE_KEY, 'Value', '< 100 = Decrease, > 100 = Increase', 100))
    params.insert(nx.Int32Parameter(ChangeColorsFilter.BRIGHTNESS_PERCENTAGE_KEY, 'Brightness', '< 100 = Decrease, > 100 = Increase', 100))
    params.insert(nx.FileSystemPathParameter(ChangeColorsFilter.OUTPUT_IMAGE_PATH_KEY, 'Output File', '', '', {'.png', '.jpg.', 'tif'}, nx.FileSystemPathParameter.PathType.OutputFile, False))

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


    input_image_path = args[ChangeColorsFilter.INPUT_IMAGE_PATH_KEY]
    output_image_path = args[ChangeColorsFilter.OUTPUT_IMAGE_PATH_KEY]
    hue_shift_degrees = args[ChangeColorsFilter.HUE_SHIFT_DEGREES_KEY]
    saturation_percentage = args[ChangeColorsFilter.SATURATION_PERCENTAGE_KEY]
    value_percentage = args[ChangeColorsFilter.VALUE_PERCENTAGE_KEY]
    brightness_percentage = args[ChangeColorsFilter.BRIGHTNESS_PERCENTAGE_KEY]

    hue_shift = int((hue_shift_degrees / 360) * 255) % 255
    saturation_factor = saturation_percentage / 100
    value_factor = value_percentage / 100
    brightness_factor = brightness_percentage / 100

    image = Image.open(input_image_path)

    hsv_image = image.convert("HSV")

    data = []
    for pixel in hsv_image.getdata():
        hue, saturation, value = pixel
        hue = (hue + hue_shift) % 255
        saturation = max(0, min(255, int(saturation * saturation_factor)))
        value = max(0, min(255, int(value * value_factor)))
        data.append((hue, saturation, value))

    hsv_image.putdata(data)

    adjusted_image = hsv_image.convert("RGB")

    enhancer = ImageEnhance.Brightness(adjusted_image)
    brightened_image = enhancer.enhance(brightness_factor)

    # brightened_image.show() 
    brightened_image.save(output_image_path)

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute Complete'))
    return nx.Result()

