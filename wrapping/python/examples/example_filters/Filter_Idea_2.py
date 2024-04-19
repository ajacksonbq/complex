
#Title

from typing import List
import simplnx as nx
from PIL import Image, ImageDraw, ImageFont

class Filter_Idea_2:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  TEST_KEY = 'test'
  POSITION = 'pos1'
  LABEL = 'label'
  FILE_PATH = 'file1'
  NEW_FILE_PATH = 'nfp1'
  LABEL_COLOR = 'color'
  FONT_SIZE = 'font'

  INPUT_POINT = 'ipoint'

  TOP_LEFT_STR = "Top Left"
  TOP_CENTER_STR = "Top Center"
  TOP_RIGHT_STR = "Top Right"

  STR_LIST = [TOP_LEFT_STR, TOP_CENTER_STR, TOP_RIGHT_STR]

  COLOR_LIST = [
    "aliceblue", "antiquewhite", "aqua", "aquamarine", "azure", "beige", "bisque",
    "black", "blanchedalmond", "blue", "blueviolet", "brown", "burlywood",
    "cadetblue", "chartreuse", "chocolate", "coral", "cornflowerblue", "cornsilk",
    "crimson", "cyan", "darkblue", "darkcyan", "darkgoldenrod", "darkgray", "darkgrey",
    "darkgreen", "darkkhaki", "darkmagenta", "darkolivegreen", "darkorange", "darkorchid",
    "darkred", "darksalmon", "darkseagreen", "darkslateblue", "darkslategray", "darkslategrey",
    "darkturquoise", "darkviolet", "deeppink", "deepskyblue", "dimgray", "dimgrey", "dodgerblue",
    "firebrick", "floralwhite", "forestgreen", "fuchsia", "gainsboro", "ghostwhite", "gold", "goldenrod",
    "gray", "grey", "green", "greenyellow", "honeydew", "hotpink", "indianred", "indigo", "ivory",
    "khaki", "lavender", "lavenderblush", "lawngreen", "lemonchiffon", "lightblue", "lightcoral",
    "lightcyan", "lightgoldenrodyellow", "lightgreen", "lightgray", "lightgrey", "lightpink",
    "lightsalmon", "lightseagreen", "lightskyblue", "lightslategray", "lightslategrey", "lightsteelblue",
    "lightyellow", "lime", "limegreen", "linen", "magenta", "maroon", "mediumaquamarine", "mediumblue",
    "mediumorchid", "mediumpurple", "mediumseagreen", "mediumslateblue", "mediumspringgreen",
    "mediumturquoise", "mediumvioletred", "midnightblue", "mintcream", "mistyrose", "moccasin",
    "navajowhite", "navy", "oldlace", "olive", "olivedrab", "orange", "orangered", "orchid",
    "palegoldenrod", "palegreen", "paleturquoise", "palevioletred", "papayawhip", "peachpuff",
    "peru", "pink", "plum", "powderblue", "purple", "rebeccapurple", "red", "rosybrown", "royalblue",
    "saddlebrown", "salmon", "sandybrown", "seagreen", "seashell", "sienna", "silver", "skyblue",
    "slateblue", "slategray", "slategrey", "snow", "springgreen", "steelblue", "tan", "teal",
    "thistle", "tomato", "turquoise", "violet", "wheat", "white", "whitesmoke", "yellow", "yellowgreen"
]

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('0a10bbde-8f88-4ec4-a185-eaece88cecc8')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'Filter_Idea_2'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'Filter_Idea_2'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'Filter_Idea_2'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  Filter_Idea_2
    """
    return Filter_Idea_2()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(Filter_Idea_2.FILE_PATH, 'Image Path', 'Original Image Path', '', {'.png'}, nx.FileSystemPathParameter.PathType.InputFile))
    # params.insert(nx.StringParameter(Filter_Idea_2.POSITION, 'Label Position', 'Enter a Postion (top_left, top_center, top_right)', 'top_center'))
    params.insert(nx.ChoicesParameter(Filter_Idea_2.POSITION, 'Label Position', 'Enter a Postion (top_left, top_center, top_right)', 0, Filter_Idea_2.STR_LIST))

    params.insert(nx.StringParameter(Filter_Idea_2.LABEL, 'Label', 'Enter a String Color', 'Cube Label'))
    params.insert(nx.Int16Parameter(Filter_Idea_2.FONT_SIZE, 'Font Size', 'Enter a Font Size', 24))
    # params.insert(nx.StringParameter(Filter_Idea_2.LABEL_COLOR, 'Label Color', 'Enter a Color', 'black'))
    params.insert(nx.ChoicesParameter(Filter_Idea_2.LABEL_COLOR, 'Label Color', 'Enter a Color', 0, Filter_Idea_2.COLOR_LIST))

    params.insert(nx.FileSystemPathParameter(Filter_Idea_2.NEW_FILE_PATH, 'Output File Path', 'Created Image Path', '', {'.png'}, nx.FileSystemPathParameter.PathType.OutputFile))
    params.insert(nx.VectorFloat64Parameter(Filter_Idea_2.INPUT_POINT, 'Input Point', 'Example float64 vector help text', [60.0, 120.0, 1000.0], ["X", "Y", "Z"]))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """

    position: str = args[Filter_Idea_2.POSITION]
    title_label: str = args[Filter_Idea_2.LABEL]
    color: str = args[Filter_Idea_2.LABEL_COLOR]
    font_size: int = args[Filter_Idea_2.FONT_SIZE]
    file_path: str = args[Filter_Idea_2.FILE_PATH]
    new_file_name: str = args[Filter_Idea_2.NEW_FILE_PATH]
    input_point: list = args[Filter_Idea_2.INPUT_POINT]

    # Create the Errors and Warnings Lists to commuicate back to the user if anything has gone wrong
    errors = []
    warnings = []
    output_actions = None

    warning = nx.Warning(-420, 'Hey this is a warning, bleh bleh bleh')
    warnings.append(warning)

    if font_size > 100:
      error = nx.Error(-8700, "Font Size > 100")
      errors.append(error)
      return nx.IFilter.PreflightResult(output_actions, errors=errors, warnings=warnings)

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight: {file_path}'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Input Point: {input_point}'))

    return nx.IFilter.PreflightResult(output_actions, errors=None, warnings=warnings)

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """
    position: str = args[Filter_Idea_2.POSITION]
    title_label: str = args[Filter_Idea_2.LABEL]
    color: str = args[Filter_Idea_2.LABEL_COLOR]
    font_size: int = args[Filter_Idea_2.FONT_SIZE]
    file_path: str = args[Filter_Idea_2.FILE_PATH]
    new_file_name: str = args[Filter_Idea_2.NEW_FILE_PATH]
    label_location_index: int = args[Filter_Idea_2.POSITION]
    color_index: int = args[Filter_Idea_2.LABEL_COLOR]

    # Open the original image
    image = Image.open(file_path)
    draw = ImageDraw.Draw(image)

    # Load a font (adjust the path as necessary)
    try:
        font = ImageFont.truetype("arial.ttf", font_size)  # Use the font_size parameter
    except IOError:
        font = ImageFont.load_default()
        print("Default font loaded, install 'arial.ttf' for better font support.")

    # Workaround to determine text size:
    temp_image = Image.new('RGB', (1000, 1000))  # Create a large enough temp image
    temp_draw = ImageDraw.Draw(temp_image)
    temp_draw.text((0, 0), title_label, font=font, fill='black')
    temp_bbox = temp_draw.textbbox((0, 0), title_label, font=font)
    text_width, text_height = temp_bbox[2] - temp_bbox[0], temp_bbox[3] - temp_bbox[1]

    # Determine the position for the title
    if Filter_Idea_2.STR_LIST[label_location_index] == Filter_Idea_2.TOP_LEFT_STR:
        title_position = (10, 10)
    elif Filter_Idea_2.STR_LIST[label_location_index] == Filter_Idea_2.TOP_CENTER_STR:
        title_position = ((image.width - text_width) / 2, 10)
    elif Filter_Idea_2.STR_LIST[label_location_index] == Filter_Idea_2.TOP_RIGHT_STR:
        title_position = (image.width - text_width - 10, 10)

    # Draw the title on the original image
    draw.text(title_position, title_label, fill=Filter_Idea_2.COLOR_LIST[color_index], font=font)

    # Determine the save path
    save_path = new_file_name if new_file_name else file_path

    # Save the modified image
    image.save(save_path)

    return nx.Result()
