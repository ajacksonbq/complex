from typing import List
import simplnx as nx

from google.oauth2.service_account import Credentials
from googleapiclient.discovery import build
import numpy as np

class ImportFromGoogleSheets:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """

  SERVICE_ACCOUNT_FILE_KEY = 'service_account'
  SPREADSHEET_ID_KEY = 'spreadsheet_id'
  SHEET_NAME_KEY = 'sheet_name'
  RANGE_START_KEY = 'range_start'
  RANGE_END_KEY = 'range_end'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('3578414e-fc1b-4ee8-942d-4ff2f2723cf0')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'ImportFromGoogleSheets'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'ImportFromGoogleSheets'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'ImportFromGoogleSheets'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  ImportFromGoogleSheets
    """
    return ImportFromGoogleSheets()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(ImportFromGoogleSheets.SERVICE_ACCOUNT_FILE_KEY, 'Service Account File', '', '', {'.json'}, nx.FileSystemPathParameter.PathType.InputFile, False))
    params.insert(nx.StringParameter(ImportFromGoogleSheets.SPREADSHEET_ID_KEY, 'Spreadsheet ID', '', ''))
    params.insert(nx.StringParameter(ImportFromGoogleSheets.SHEET_NAME_KEY, 'Sheet Name', '', 'Sheet1'))
    params.insert(nx.StringParameter(ImportFromGoogleSheets.RANGE_START_KEY, 'Range Start', '', 'A1'))
    params.insert(nx.StringParameter(ImportFromGoogleSheets.RANGE_END_KEY, 'Range End', '', 'F10'))

    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """

    SCOPES = ['https://www.googleapis.com/auth/spreadsheets']
    SERVICE_ACCOUNT_FILE: str = args[ImportFromGoogleSheets.SERVICE_ACCOUNT_FILE_KEY]
    SPREADSHEET_ID: str = args[ImportFromGoogleSheets.SPREADSHEET_ID_KEY]
    SHEET_NAME: str = args[ImportFromGoogleSheets.SHEET_NAME_KEY]
    RANGE_START: str = args[ImportFromGoogleSheets.RANGE_START_KEY]
    RANGE_END: str = args[ImportFromGoogleSheets.RANGE_END_KEY]
    SAMPLE_RANGE = f'{SHEET_NAME}!{RANGE_START}:{RANGE_END}'

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'SCOPES = {SCOPES}'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'SERVICE ACCOUNT STRING = {SERVICE_ACCOUNT_FILE}'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'SPREADSHEET ID = {SPREADSHEET_ID}'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'SAMPLE_RANGE START = {RANGE_START}'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'SAMPLE_RANGE END = {RANGE_END}'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'SAMPLE_RANGE = {SAMPLE_RANGE}'))

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight Success'))
    return nx.IFilter.PreflightResult()

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """
    SCOPES = ['https://www.googleapis.com/auth/spreadsheets']
    SERVICE_ACCOUNT_FILE: str = args[ImportFromGoogleSheets.SERVICE_ACCOUNT_FILE_KEY]
    SPREADSHEET_ID: str = args[ImportFromGoogleSheets.SPREADSHEET_ID_KEY]
    SHEET_NAME: str = args[ImportFromGoogleSheets.SHEET_NAME_KEY]
    RANGE_START: str = args[ImportFromGoogleSheets.RANGE_START_KEY]
    RANGE_END: str = args[ImportFromGoogleSheets.RANGE_END_KEY]
    SAMPLE_RANGE = f'{SHEET_NAME}!{RANGE_START}:{RANGE_END}'

    credentials = Credentials.from_service_account_file(
        SERVICE_ACCOUNT_FILE, scopes=SCOPES)
    service = build('sheets', 'v4', credentials=credentials)

    # Call the Sheets API and read the data
    sheet = service.spreadsheets()
    result = sheet.values().get(spreadsheetId=SPREADSHEET_ID,
                                range=SAMPLE_RANGE).execute()
    values = result.get('values', [])

    letter1, letter2 = RANGE_START[0], RANGE_END[0]
    position1, position2 = ord(letter1) - ord('A'), ord(letter2) - ord('A')
    distance = abs(position2 - position1) + 1

    np_arrays = [np.array([row[i] if len(row) > i else None for row in values[1:]]) for i in range(min(distance, len(values[0])))]

    for i, arr in enumerate(np_arrays):
        message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Array {i+1}: {arr}'))

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Total Arrays Created: {len(np_arrays)}'))

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute Complete'))
    return nx.Result()