from typing import List
import simplnx as nx
import cv2
import os
import time

class CreateImageStackMovie:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  SECONDS_PER_IMAGE_KEY = 'seconds_per_image'
  INPUT_DIR_KEY = 'input_key'
  OUTPUT_FILE_KEY = 'output_key'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('2f268d59-385a-4d3c-81e4-9a82f15962af')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'CreateImageStackMovie'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'CreateImageStackMovie'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'CreateImageStackMovie'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  CreateImageStackMovie
    """
    return CreateImageStackMovie()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(CreateImageStackMovie.INPUT_DIR_KEY, 'Image Stack Directory', 'Directory where all Images are Located', '', {'.*'}, nx.FileSystemPathParameter.PathType.InputDir, False))
    params.insert(nx.Int32Parameter(CreateImageStackMovie.SECONDS_PER_IMAGE_KEY, 'Seconds Per Image', 'Amount of Seconds an Image is Displayed in the Video (Int)', 2))
    params.insert(nx.FileSystemPathParameter(CreateImageStackMovie.OUTPUT_FILE_KEY, 'Output File', 'Name the File within the Path', '', {'.mp4'}, nx.FileSystemPathParameter.PathType.OutputFile, False))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """

    errors = []
    warnings = []
    output_actions = None

    display_duration: int = args[CreateImageStackMovie.SECONDS_PER_IMAGE_KEY]
    directory_path = str(args[CreateImageStackMovie.INPUT_DIR_KEY])
    files = os.listdir(directory_path)
    image_files = [os.path.join(directory_path, f) for f in files if os.path.isfile(os.path.join(directory_path, f))]
    frame = cv2.imread(image_files[0])
        
    total_images = len(image_files)
    frame_height, frame_width, layers = frame.shape

    if display_duration == 0:
      error = nx.Error(-1235, f'Seconds Per Image must be Greater than 0')
      errors.append(error)
      return nx.IFilter.PreflightResult(output_actions, errors=errors, warnings=warnings)

    if frame_height != frame_width:
      message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'if Resolution is not 1:1 Aspect Ratio, Image could Stretch or Compress'))

    if total_images == 0:
      error = nx.Error(-1245, f"No images found in the directory.")
      errors.append(error)
      return nx.IFilter.PreflightResult(output_actions, errors=errors, warnings=warnings)

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight Success'))
    return nx.IFilter.PreflightResult(output_actions, errors=None, warnings=warnings)

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """
    display_duration: int = args[CreateImageStackMovie.SECONDS_PER_IMAGE_KEY]
    directory_path = str(args[CreateImageStackMovie.INPUT_DIR_KEY])
    output_filename = str(args[CreateImageStackMovie.OUTPUT_FILE_KEY])
    start_time = time.time()
    files = os.listdir(directory_path)
    image_files = [os.path.join(directory_path, f) for f in files if os.path.isfile(os.path.join(directory_path, f))]
    total_images = len(image_files)

    last_output_time = start_time - 1

    frame_rate = 1.0 / display_duration

    frame = cv2.imread(image_files[0])

    frame_height, frame_width, layers = frame.shape

    fourcc = cv2.VideoWriter_fourcc(*'mp4v')
    video = cv2.VideoWriter(output_filename, fourcc, frame_rate, (frame_width, frame_height))

    for i, image_file in enumerate(image_files, 1):
        frame = cv2.imread(image_file)
        if frame is not None:
            video.write(frame)
            
        else:
            message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Warning, f"Skipping file {image_file}: failed to load."))

        current_time = time.time()
        if current_time - last_output_time >= 1:  # Only update if at least 1 second has passed
            elapsed_time = current_time - start_time
            estimated_total_time = (elapsed_time / i) * total_images
            estimated_remaining_time = estimated_total_time - elapsed_time
            message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f"Processing file {i}/{total_images}. Estimated time remaining: {estimated_remaining_time:.2f} seconds."))
            last_output_time = current_time

    video.release()

    total_time = time.time() - start_time
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f"Total processing time: {total_time:.2f} seconds."))

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute Complete'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'File Created at {output_filename}'))
    return nx.Result()
