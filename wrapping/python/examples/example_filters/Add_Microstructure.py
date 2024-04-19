
from typing import List
import simplnx as nx
from PIL import Image, ImageDraw, ImageFont

class Add_Microstructure:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  FILE_PATH = 'file'
  SIZE = 'size'
  TICKS = 'ticks'
  POSITION = 'position'
  LABEL = 'label'
  NEW_FILE_PATH = 'file1'
  COLOR = 'color'
  TICK_LABEL_SIZE = 'tlsize'
  BAR_LABEL_SIZE = 'barlabelsize'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('7c9cc0b5-71bc-49b7-b727-7209ac15585a')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'Add_Microstructure'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'Add_Microstructure'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'Add_Microstructure'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  Add_Microstructure
    """
    return Add_Microstructure()


  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(Add_Microstructure.FILE_PATH, 'Image Path', 'Original Image Path', '', {'.png'}, nx.FileSystemPathParameter.PathType.InputFile))
    params.insert(nx.FileSystemPathParameter(Add_Microstructure.NEW_FILE_PATH, 'Output File Path', 'Created Image Path', '', {'.png'}, nx.FileSystemPathParameter.PathType.OutputFile))
    params.insert(nx.StringParameter(Add_Microstructure.LABEL, 'Label', 'Enter Label of Micron Bar', 'Micron Bar'))
    params.insert(nx.Int16Parameter(Add_Microstructure.SIZE, 'Micron Bar Size', 'Enter a Border Size', 130))
    params.insert(nx.Int16Parameter(Add_Microstructure.TICKS, '# of Ticks', 'Enter how many ticks you want in the micron bar', 7))
    params.insert(nx.Int16Parameter(Add_Microstructure.TICK_LABEL_SIZE, 'Tick Size', 'Size of ticks', 10))
    params.insert(nx.Int16Parameter(Add_Microstructure.BAR_LABEL_SIZE, 'Bar Label Size', 'Enter a Label Size', 12))
    params.insert(nx.StringParameter(Add_Microstructure.COLOR, 'Color', 'Enter the color of the Micron Bar', 'black'))
    params.insert(nx.StringParameter(Add_Microstructure.POSITION, 'Position', 'Enter a Position for the bar (bottom_left, bottom_middle, bottom_right)', 'bottom_left'))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """
    file_path: str = args[Add_Microstructure.FILE_PATH]
    new_file_name: str = args[Add_Microstructure.NEW_FILE_PATH]
    size: int = args[Add_Microstructure.SIZE]
    ticks: int = args[Add_Microstructure.TICKS]
    tick_label_size: int = args[Add_Microstructure.TICK_LABEL_SIZE]
    bar_label_size: int = args[Add_Microstructure.BAR_LABEL_SIZE]
    color: str = args[Add_Microstructure.COLOR]
    position: str = args[Add_Microstructure.POSITION]
    label: str = args[Add_Microstructure.LABEL]

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight: {file_path}'))
    return nx.IFilter.PreflightResult()

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """
    file_path: str = args[Add_Microstructure.FILE_PATH]
    new_file_name: str = args[Add_Microstructure.NEW_FILE_PATH]
    size: int = args[Add_Microstructure.SIZE]
    ticks: int = args[Add_Microstructure.TICKS]
    tick_label_size: int = args[Add_Microstructure.TICK_LABEL_SIZE]
    bar_label_size: int = args[Add_Microstructure.BAR_LABEL_SIZE]
    color: str = args[Add_Microstructure.COLOR]
    position: str = args[Add_Microstructure.POSITION]
    label: str = args[Add_Microstructure.LABEL]

    # Open the original image
    image = Image.open(file_path)
    draw = ImageDraw.Draw(image)
    
    # Load a font
    try:
        font = ImageFont.truetype("arial.ttf", tick_label_size)
        label_font = ImageFont.truetype("arial.ttf", bar_label_size)
    except IOError:
        font = ImageFont.load_default()
        label_font = ImageFont.load_default()
        print("Default font loaded, install 'arial.ttf' for better font support.")
    
    # Determine the position for the microbar
    if position == 'bottom_left':
        start_pos = (10, image.height - 50)
    elif position == 'bottom_middle':
        start_pos = (image.width // 2 - size // 2, image.height - 50)
    elif position == 'bottom_right':
        start_pos = (image.width - size - 10, image.height - 50)
    
    # Draw the microbar
    end_pos = (start_pos[0] + size, start_pos[1])
    draw.line([start_pos, end_pos], fill=color, width=2)
    
    # Draw ticks and labels
    tick_spacing = size / (ticks - 1)
    for i in range(ticks):
        tick_start = (start_pos[0] + i * tick_spacing, start_pos[1] - 5)
        tick_end = (start_pos[0] + i * tick_spacing, start_pos[1] + 5)
        draw.line([tick_start, tick_end], fill=color, width=2)
        # Draw tick labels
        draw.text((tick_start[0] - 5, tick_start[1] + 10), str(i), fill=color, font=font)
    
    # Draw microbar label
    draw.text((start_pos[0] + size // 2 - 20, start_pos[1] - 30), label, fill=color, font=label_font)
    
    # Determine the save path
    save_path = new_file_name if new_file_name else file_path
    
    # Save the modified image
    image.save(save_path)

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute: {file_path}'))
    return nx.Result()


