from typing import List
import simplnx as nx

from google.oauth2.service_account import Credentials
from googleapiclient.discovery import build
import csv

class ExportDataToGoogleSheets:
  """
  This section should contain the 'keys' that store each parameter. The value of the key should be snake_case. The name
  of the value should be ALL_CAPITOL_KEY
  """
  INPUT_FILE_KEY = 'input_file'
  SERVICE_ACCOUNT_FILE_KEY = 'service_account_file'
  SPREADSHEET_ID_KEY = 'spreadsheet_id'
  SHEET_NAME_KEY = 'sheet_name_key'
  RANGE_START_KEY = 'range_start_key'

  def uuid(self) -> nx.Uuid:
    """This returns the UUID of the filter. Each filter has a unique UUID value
    :return: The Filter's Uuid value
    :rtype: string
    """
    return nx.Uuid('85c902c1-6fe4-4b69-b267-1485e3e30f28')

  def human_name(self) -> str:
    """This returns the name of the filter as a user of DREAM3DNX would see it
    :return: The filter's human name
    :rtype: string
    """
    return 'ExportDataToGoogleSheets'

  def class_name(self) -> str:
    """The returns the name of the class that implements the filter
    :return: The name of the implementation class
    :rtype: string
    """
    return 'ExportDataToGoogleSheets'

  def name(self) -> str:
    """The returns the name of filter
    :return: The name of the filter
    :rtype: string
    """
    return 'ExportDataToGoogleSheets'

  def default_tags(self) -> List[str]:
    """This returns the default tags for this filter
    :return: The default tags for the filter
    :rtype: list
    """
    return ['python']

  def clone(self):
    """Clones the filter
    :return: A new instance of the filter
    :rtype:  ExportDataToGoogleSheets
    """
    return ExportDataToGoogleSheets()

  def parameters(self) -> nx.Parameters:
    """This function defines the parameters that are needed by the filter. Parameters collect the values from the user
       or through a pipeline file.
    """
    params = nx.Parameters()

    params.insert(nx.FileSystemPathParameter(ExportDataToGoogleSheets.INPUT_FILE_KEY, 'Input File', '', '', {'.csv'}, nx.FileSystemPathParameter.PathType.InputFile, False))
    params.insert(nx.FileSystemPathParameter(ExportDataToGoogleSheets.SERVICE_ACCOUNT_FILE_KEY, 'Service Account File', '', '', {'.json'}, nx.FileSystemPathParameter.PathType.InputFile, False))
    params.insert(nx.StringParameter(ExportDataToGoogleSheets.SPREADSHEET_ID_KEY, 'Spreadsheet ID', '', ''))
    params.insert(nx.StringParameter(ExportDataToGoogleSheets.SHEET_NAME_KEY, 'Sheet Name (Within the Sheet)', '', 'Sheet1'))
    params.insert(nx.StringParameter(ExportDataToGoogleSheets.RANGE_START_KEY, 'Start Writing in Cell: ', '', 'A1'))



    return params

  def preflight_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.PreflightResult:
    """This method preflights the filter and should ensure that all inputs are sanity checked as best as possible. Array
    sizes can be checked if the arrays are actually know at preflight time. Some filters will not be able to report output
    array sizes during preflight (segmentation filters for example).
    :returns:
    :rtype: nx.IFilter.PreflightResult
    """

    SHEET_NAME: str = args[ExportDataToGoogleSheets.SHEET_NAME_KEY]
    range_start: str = args[ExportDataToGoogleSheets.RANGE_START_KEY]
    range_to_write: str = f'{SHEET_NAME}!{range_start}'

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Range Start: {range_start}'))
    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Sheet Name: {SHEET_NAME}'))

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Preflight Success'))
    return nx.IFilter.PreflightResult()

  def execute_impl(self, data_structure: nx.DataStructure, args: dict, message_handler: nx.IFilter.MessageHandler, should_cancel: nx.AtomicBoolProxy) -> nx.IFilter.ExecuteResult:
    """ This method actually executes the filter algorithm and reports results.
    :returns:
    :rtype: nx.IFilter.ExecuteResult
    """

    SERVICE_ACCOUNT_FILE: str = args[ExportDataToGoogleSheets.SERVICE_ACCOUNT_FILE_KEY]
    SCOPES = ['https://www.googleapis.com/auth/spreadsheets']
    SAMPLE_SPREADSHEET_ID: str = args[ExportDataToGoogleSheets.SPREADSHEET_ID_KEY]
    SHEET_NAME: str = args[ExportDataToGoogleSheets.SHEET_NAME_KEY]
    csv_file_path: str = args[ExportDataToGoogleSheets.INPUT_FILE_KEY]
    range_start: str = args[ExportDataToGoogleSheets.RANGE_START_KEY]
    range_to_write: str = f'{SHEET_NAME}!{range_start}'


    credentials = Credentials.from_service_account_file(
        SERVICE_ACCOUNT_FILE, scopes=SCOPES)
    service = build('sheets', 'v4', credentials=credentials)

    with open(csv_file_path, mode='r', encoding='utf-8') as csv_file:
        csv_reader = csv.reader(csv_file)
        data = list(csv_reader)

    # Prepare the data for the Sheets API
    body = {
        'values': data
    }

    # Call the Sheets API and write the CSV data
    result = service.spreadsheets().values().update(
        spreadsheetId=SAMPLE_SPREADSHEET_ID, range=range_to_write,
        valueInputOption='RAW', body=body).execute()

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f"{result.get('updatedCells')} cells updated."))

    message_handler(nx.IFilter.Message(nx.IFilter.Message.Type.Info, f'Execute Complete'))
    return nx.Result()

