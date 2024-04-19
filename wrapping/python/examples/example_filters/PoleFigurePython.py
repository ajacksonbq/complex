from typing import List
import simplnx as nx

import itkimageprocessing as nxitk
import orientationanalysis as nxor

import numpy as np

import os
import shutil
from pathlib import Path



#------------------------------------------------------------------------------
# Create a Data Structure
#------------------------------------------------------------------------------
data_structure = nx.DataStructure()

# Import a CTF Data file
result = nxor.ReadCtfDataFilter.execute(data_structure=data_structure, 
                            cell_attribute_matrix_name="CellData", 
                            cell_ensemble_attribute_matrix_name="Phase Data",
                            data_container_name=nx.DataPath(["DataContainer"]), 
                            input_file="G:/MIDAS_625/result_fit_382.ctf")

# check_filter_result(nxor.ReadCtfDataFilter, result)

#------------------------------------------------------------------------------
# Rotate the Sample Reference Frame 180@010
#------------------------------------------------------------------------------
result = nx.RotateSampleRefFrameFilter.execute(data_structure=data_structure,
remove_original_geometry=True,
rotate_slice_by_slice=False,
rotation_axis=[0.0,1.0,0.0,180.0],
rotation_representation=0,
selected_image_geometry=nx.DataPath(["DataContainer"])
)
# check_filter_result(nx.RotateSampleRefFrameFilter, result)