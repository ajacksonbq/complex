#include "WritePoleFigureFilter.hpp"

#include "OrientationAnalysis/Filters/Algorithms/WritePoleFigure.hpp"
#include "OrientationAnalysis/utilities/Fonts.hpp"
#include "OrientationAnalysis/utilities/LatoBold.hpp"

#include "complex/DataStructure/DataPath.hpp"
#include "complex/DataStructure/StringArray.hpp"
#include "complex/Filter/Actions/CreateArrayAction.hpp"
#include "complex/Filter/Actions/CreateImageGeometryAction.hpp"
#include "complex/Filter/Actions/DeleteDataAction.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/BoolParameter.hpp"
#include "complex/Parameters/ChoicesParameter.hpp"
#include "complex/Parameters/DataGroupCreationParameter.hpp"
#include "complex/Parameters/DataPathSelectionParameter.hpp"
#include "complex/Parameters/FileSystemPathParameter.hpp"
#include "complex/Parameters/NumberParameter.hpp"
#include "complex/Parameters/StringParameter.hpp"

#include <canvas_ity.hpp>

#include <filesystem>
namespace fs = std::filesystem;

using namespace complex;

namespace
{

/**
 * @brief
 * @param imageSize
 * @param fontPtSize
 * @return
 */
float32 GetXCharWidth(int32 imageSize, float32 fontPtSize)
{
  std::vector<unsigned char> m_LatoBold;
  fonts::Base64Decode(fonts::k_LatoBoldBase64, m_LatoBold);

  canvas_ity::canvas tempContext(imageSize, imageSize);
  tempContext.set_font(m_LatoBold.data(), static_cast<int>(m_LatoBold.size()), fontPtSize);
  return tempContext.measure_text("X");
}
} // namespace

namespace complex
{
//------------------------------------------------------------------------------
std::string WritePoleFigureFilter::name() const
{
  return FilterTraits<WritePoleFigureFilter>::name.str();
}

//------------------------------------------------------------------------------
std::string WritePoleFigureFilter::className() const
{
  return FilterTraits<WritePoleFigureFilter>::className;
}

//------------------------------------------------------------------------------
Uuid WritePoleFigureFilter::uuid() const
{
  return FilterTraits<WritePoleFigureFilter>::uuid;
}

//------------------------------------------------------------------------------
std::string WritePoleFigureFilter::humanName() const
{
  return "Generate and Write Pole Figure Images";
}

//------------------------------------------------------------------------------
std::vector<std::string> WritePoleFigureFilter::defaultTags() const
{
  return {className(), "IO", "Output", "Write", "Export", "EBSD", "Pole Figure"};
}

//------------------------------------------------------------------------------
Parameters WritePoleFigureFilter::parameters() const
{
  Parameters params;
  // Create the parameter descriptors that are needed for this filter
  params.insertSeparator(Parameters::Separator{"Input Parameters"});
  params.insert(std::make_unique<StringParameter>(k_Title_Key, "Figure Title", "The title to place at the top of the Pole Figure", "Figure Title"));
  params.insert(std::make_unique<Int32Parameter>(k_ImageSize_Key, "Image Size (Square Pixels)", "The number of pixels that define the height and width of **each** output pole figure", 512));
  params.insert(std::make_unique<ChoicesParameter>(k_ImageLayout_Key, "Image Layout", "How to layout the 3 pole figures. 0=Horizontal, 1=Vertical, 2=Square", 0,
                                                   ChoicesParameter::Choices{"Horizontal", "Vertical", "Square"}));

  params.insertLinkableParameter(std::make_unique<ChoicesParameter>(k_GenerationAlgorithm_Key, "Pole Figure Type", "The type of pole figure generated. 0=Color, 1=Discrete", 0,
                                                                    ChoicesParameter::Choices{"Color Intensity", "Discrete"}));
  params.insert(std::make_unique<Int32Parameter>(k_LambertSize_Key, "Lambert Image Size (Pixels)", "The height/width of the internal Lambert Square that is used for interpolation", 64));
  params.insert(std::make_unique<Int32Parameter>(k_NumColors_Key, "Number of Colors", "The number of colors to use for the Color Intensity pole figures", 32));

  params.insertSeparator(Parameters::Separator{"Required Input Cell Data"});
  params.insert(std::make_unique<ArraySelectionParameter>(k_CellEulerAnglesArrayPath_Key, "Euler Angles", "Three angles defining the orientation of the Element in Bunge convention (Z-X-Z)",
                                                          DataPath{}, ArraySelectionParameter::AllowedTypes{DataType::float32}, ArraySelectionParameter::AllowedComponentShapes{{3}}));
  params.insert(std::make_unique<ArraySelectionParameter>(k_CellPhasesArrayPath_Key, "Phases", "Specifies to which Ensemble each cell belongs", DataPath{},
                                                          ArraySelectionParameter::AllowedTypes{DataType::int32}, ArraySelectionParameter::AllowedComponentShapes{{1}}));
  params.insertSeparator(Parameters::Separator{"Optional Data Mask"});
  params.insertLinkableParameter(std::make_unique<BoolParameter>(k_UseMask_Key, "Use Mask Array", "Should the algorithm use a mask array to remove non-indexed points", false));
  params.insert(std::make_unique<ArraySelectionParameter>(k_MaskArrayPath_Key, "Mask Array", "DataPath to the input Mask DataArray", DataPath({"Mask"}),
                                                          ArraySelectionParameter::AllowedTypes{DataType::boolean, DataType::uint8}, ArraySelectionParameter::AllowedComponentShapes{{1}}));
  params.insertSeparator(Parameters::Separator{"Required Input Cell Ensemble Data"});
  params.insert(std::make_unique<ArraySelectionParameter>(k_CrystalStructuresArrayPath_Key, "Crystal Structures", "Enumeration representing the crystal structure for each Ensemble",
                                                          DataPath({"Ensemble Data", "CrystalStructures"}), ArraySelectionParameter::AllowedTypes{DataType::uint32},
                                                          ArraySelectionParameter::AllowedComponentShapes{{1}}));
  params.insert(std::make_unique<DataPathSelectionParameter>(k_MaterialNameArrayPath_Key, "Material Name", "DataPath to the input DataArray that holds the material names", DataPath{}));

  params.insertSeparator(Parameters::Separator{"Created Objects/Output File Parameters"});
  params.insertLinkableParameter(std::make_unique<BoolParameter>(k_WriteImageToDisk, "Write Pole Figure as Image", "Should the filter write the pole figure plots to a file.", true));

  params.insert(std::make_unique<FileSystemPathParameter>(k_OutputPath_Key, "Output Directory Path",
                                                          "This is the path to the directory where the pole figures will be created. One file for each phase.", fs::path(""),
                                                          FileSystemPathParameter::ExtensionsType{}, FileSystemPathParameter::PathType::OutputDir, true));
  params.insert(
      std::make_unique<StringParameter>(k_ImagePrefix_Key, "Pole Figure File Prefix", "The prefix to apply to each generated pole figure. Each Phase will have its own pole figure.", "Phase_"));

  params.insertLinkableParameter(std::make_unique<BoolParameter>(k_SaveAsImageGeometry_Key, "Save Output as Image Geometry", "Save the generated pole figure as an ImageGeometry", true));
  params.insert(std::make_unique<DataGroupCreationParameter>(k_ImageGeometryPath_Key, "Created Image Geometry", "The path to the created Image Geometry", DataPath({"PoleFigure"})));

  // Associate the Linkable Parameter(s) to the children parameters that they control
  params.linkParameters(k_UseMask_Key, k_MaskArrayPath_Key, true);

  params.linkParameters(k_GenerationAlgorithm_Key, k_LambertSize_Key, std::make_any<ChoicesParameter::ValueType>(0));
  params.linkParameters(k_GenerationAlgorithm_Key, k_NumColors_Key, std::make_any<ChoicesParameter::ValueType>(0));

  params.linkParameters(k_SaveAsImageGeometry_Key, k_ImageGeometryPath_Key, true);
  params.linkParameters(k_WriteImageToDisk, k_OutputPath_Key, true);
  params.linkParameters(k_WriteImageToDisk, k_ImagePrefix_Key, true);

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer WritePoleFigureFilter::clone() const
{
  return std::make_unique<WritePoleFigureFilter>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult WritePoleFigureFilter::preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler,
                                                              const std::atomic_bool& shouldCancel) const
{

  auto pTitleValue = filterArgs.value<StringParameter::ValueType>(k_Title_Key);
  auto pImageLayoutValue = filterArgs.value<ChoicesParameter::ValueType>(k_ImageLayout_Key);
  auto pOutputPathValue = filterArgs.value<FileSystemPathParameter::ValueType>(k_OutputPath_Key);
  auto pImagePrefixValue = filterArgs.value<StringParameter::ValueType>(k_ImagePrefix_Key);
  auto pImageSizeValue = filterArgs.value<int32>(k_ImageSize_Key);
  auto pCellEulerAnglesArrayPathValue = filterArgs.value<DataPath>(k_CellEulerAnglesArrayPath_Key);
  auto pCellPhasesArrayPathValue = filterArgs.value<DataPath>(k_CellPhasesArrayPath_Key);
  auto pGoodVoxelsArrayPathValue = filterArgs.value<DataPath>(k_MaskArrayPath_Key);
  auto pCrystalStructuresArrayPathValue = filterArgs.value<DataPath>(k_CrystalStructuresArrayPath_Key);
  auto pMaterialNameArrayPathValue = filterArgs.value<DataPathSelectionParameter::ValueType>(k_MaterialNameArrayPath_Key);

  auto pSaveAsImageGeometry = filterArgs.value<bool>(k_SaveAsImageGeometry_Key);
  auto pWriteImageToDisk = filterArgs.value<bool>(k_WriteImageToDisk);
  auto pOutputImageGeometryPath = filterArgs.value<DataPath>(k_ImageGeometryPath_Key);

  PreflightResult preflightResult;

  const auto* materialNamePtr = dataStructure.getDataAs<StringArray>(pMaterialNameArrayPathValue);
  if(nullptr == materialNamePtr)
  {
    return {MakeErrorResult<OutputActions>(-680000, fmt::format("MaterialNames DataArray should be of type 'StringArray'. Selected array was '{}'", pMaterialNameArrayPathValue.toString())), {}};
  }

  complex::Result<OutputActions> resultOutputActions;

  // Roughly calculate the output dimensions of the ImageGeometry. This may change
  // in small amounts due to the XCharWidth not being calculated.
  float32 fontPtSize = pImageSizeValue / 16.0f;
  float32 margins = pImageSizeValue / 32.0f;

  float32 xCharWidth = GetXCharWidth(pImageSizeValue, fontPtSize);

  int32 pageWidth = 0;
  int32 pageHeight = margins + fontPtSize;
  // Each Pole Figure gets its own Square mini canvas to draw into.
  float32 subCanvasWidth = margins + pImageSizeValue + xCharWidth + margins;
  float32 subCanvasHeight = margins + fontPtSize + pImageSizeValue + fontPtSize * 2 + margins * 2;
  if(static_cast<WritePoleFigure::LayoutType>(pImageLayoutValue) == WritePoleFigure::LayoutType::Horizontal)
  {
    pageWidth = subCanvasWidth * 4;
    pageHeight = pageHeight + subCanvasHeight;
  }
  else if(static_cast<WritePoleFigure::LayoutType>(pImageLayoutValue) == WritePoleFigure::LayoutType::Vertical)
  {
    pageWidth = subCanvasWidth;
    pageHeight = pageHeight + subCanvasHeight * 4.0f;
  }
  else if(static_cast<WritePoleFigure::LayoutType>(pImageLayoutValue) == complex::WritePoleFigure::LayoutType::Square)
  {
    pageWidth = subCanvasWidth * 2.0f;
    pageHeight = pageHeight + subCanvasHeight * 2.0f;
  }
  const std::vector<size_t> dims = {static_cast<usize>(pageWidth), static_cast<usize>(pageHeight), 1ULL};
  auto createImageGeometryAction =
      std::make_unique<CreateImageGeometryAction>(pOutputImageGeometryPath, dims, std::vector<float>{0.0f, 0.0f, 0.0f}, std::vector<float>{1.0f, 1.0f, 1.0f}, write_pole_figure::k_ImageAttrMatName);
  resultOutputActions.value().appendAction(std::move(createImageGeometryAction));

  if(!pSaveAsImageGeometry)
  {
    // After the execute function has been done, delete the original image geometry
    resultOutputActions.value().appendDeferredAction(std::make_unique<DeleteDataAction>(pOutputImageGeometryPath));
  }

  std::vector<PreflightValue> preflightUpdatedValues;
  if(pWriteImageToDisk)
  {
    preflightUpdatedValues.push_back({"Example Output File.", fmt::format("{}/{}Phase_1.tiff", pOutputPathValue.string(), pImagePrefixValue)});
  }

  // Return both the resultOutputActions and the preflightUpdatedValues via std::move()
  return {std::move(resultOutputActions), std::move(preflightUpdatedValues)};
}

//------------------------------------------------------------------------------
Result<> WritePoleFigureFilter::executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                            const std::atomic_bool& shouldCancel) const
{

  WritePoleFigureInputValues inputValues;

  inputValues.Title = filterArgs.value<StringParameter::ValueType>(k_Title_Key);
  inputValues.GenerationAlgorithm = filterArgs.value<ChoicesParameter::ValueType>(k_GenerationAlgorithm_Key);
  inputValues.LambertSize = filterArgs.value<int32>(k_LambertSize_Key);
  inputValues.NumColors = filterArgs.value<int32>(k_NumColors_Key);
  inputValues.ImageLayout = filterArgs.value<ChoicesParameter::ValueType>(k_ImageLayout_Key);
  inputValues.OutputPath = filterArgs.value<FileSystemPathParameter::ValueType>(k_OutputPath_Key);
  inputValues.ImagePrefix = filterArgs.value<StringParameter::ValueType>(k_ImagePrefix_Key);
  inputValues.ImageSize = filterArgs.value<int32>(k_ImageSize_Key);
  inputValues.UseMask = filterArgs.value<bool>(k_UseMask_Key);
  inputValues.CellEulerAnglesArrayPath = filterArgs.value<DataPath>(k_CellEulerAnglesArrayPath_Key);
  inputValues.CellPhasesArrayPath = filterArgs.value<DataPath>(k_CellPhasesArrayPath_Key);
  inputValues.MaskArrayPath = filterArgs.value<DataPath>(k_MaskArrayPath_Key);
  inputValues.CrystalStructuresArrayPath = filterArgs.value<DataPath>(k_CrystalStructuresArrayPath_Key);
  inputValues.MaterialNameArrayPath = filterArgs.value<DataPathSelectionParameter::ValueType>(k_MaterialNameArrayPath_Key);
  inputValues.SaveAsImageGeometry = filterArgs.value<bool>(k_SaveAsImageGeometry_Key);
  inputValues.WriteImageToDisk = filterArgs.value<bool>(k_WriteImageToDisk);
  inputValues.OutputImageGeometryPath = filterArgs.value<DataPath>(k_ImageGeometryPath_Key);

  return WritePoleFigure(dataStructure, messageHandler, shouldCancel, &inputValues)();
}
} // namespace complex
