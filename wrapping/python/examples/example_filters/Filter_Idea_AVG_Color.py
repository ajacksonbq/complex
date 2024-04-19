
from typing import List
import simplnx as nx
from PIL import Image


class Filter_Idea_AVG_Color:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  FILE_PATH = 'file1'
  BORDER_SIZE = 'bordersize'
  NEW_FILE_PATH = 'file2'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('162b3f98-55e0-4bb8-a5d4-2097c324f650')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'Filter_Idea_AVG_Color'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'Filter_Idea_AVG_Color'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'Filter_Idea_AVG_Color'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  Filter_Idea_AVG_Color
    """
    return Filter_Idea_AVG_Color()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(Filter_Idea_AVG_Color.FILE_PATH, 'Image Path', 'Original Image Path', '', {'.png'}, nx.FileSystemPathParameter.PathType.InputFile))
    params.insert(nx.Int16Parameter(Filter_Idea_AVG_Color.BORDER_SIZE, 'Border Size', 'Enter a Border Size', 200))
    params.insert(nx.FileSystemPathParameter(Filter_Idea_AVG_Color.NEW_FILE_PATH, 'Output File Path', 'Created Image Path', '', {'.png'}, nx.FileSystemPathParameter.PathType.OutputFile))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """
    file_path: str = args[Filter_Idea_AVG_Color.FILE_PATH]
    border_size: int = args[Filter_Idea_AVG_Color.BORDER_SIZE]
    new_file_name: str = args[Filter_Idea_AVG_Color.NEW_FILE_PATH]
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight: {file_path}'))
    return nx.IFilter.PreflightResult()

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """

    file_path: str = args[Filter_Idea_AVG_Color.FILE_PATH]
    border_size: int = args[Filter_Idea_AVG_Color.BORDER_SIZE]
    new_file_name: str = args[Filter_Idea_AVG_Color.NEW_FILE_PATH]
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight: {file_path}'))

    # Open the original image
    image = Image.open(file_path)
    
    # Calculate the average color
    pixels = list(image.getdata())
    avg_color = tuple(sum(col) // len(col) for col in zip(*pixels))
    
    # Calculate the size of the new image (original size + border)
    new_width = image.width + 2 * border_size
    new_height = image.height + 2 * border_size
    
    # Create a new image with the average color
    new_image = Image.new("RGB", (new_width, new_height), avg_color)
    
    # Paste the original image onto the center of the new image
    new_image.paste(image, (border_size, border_size))
    
    # Determine the save path
    save_path = new_file_name if new_file_name else file_path
    
    # Save the modified image
    new_image.save(save_path)

    return nx.Result()
