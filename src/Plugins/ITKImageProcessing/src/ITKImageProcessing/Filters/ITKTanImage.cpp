#include "ITKTanImage.hpp"

#include "ITKImageProcessing/Common/ITKArrayHelper.hpp"
#include "ITKImageProcessing/Common/sitkCommon.hpp"

#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/DataObjectNameParameter.hpp"
#include "complex/Parameters/GeometrySelectionParameter.hpp"

#include <itkTanImageFilter.h>

using namespace complex;

namespace cxITKTanImage
{
using ArrayOptionsType = ITK::ScalarPixelIdTypeList;
// VectorPixelIDTypeList;

struct ITKTanImageFunctor
{
  template <class InputImageT, class OutputImageT, uint32 Dimension>
  auto createFilter() const
  {
    using FilterType = itk::TanImageFilter<InputImageT, OutputImageT>;
    auto filter = FilterType::New();
    return filter;
  }
};
} // namespace cxITKTanImage

namespace complex
{
//------------------------------------------------------------------------------
std::string ITKTanImage::name() const
{
  return FilterTraits<ITKTanImage>::name;
}

//------------------------------------------------------------------------------
std::string ITKTanImage::className() const
{
  return FilterTraits<ITKTanImage>::className;
}

//------------------------------------------------------------------------------
Uuid ITKTanImage::uuid() const
{
  return FilterTraits<ITKTanImage>::uuid;
}

//------------------------------------------------------------------------------
std::string ITKTanImage::humanName() const
{
  return "ITK Tan Image Filter";
}

//------------------------------------------------------------------------------
std::vector<std::string> ITKTanImage::defaultTags() const
{
  return {className(), "ITKImageProcessing", "ITKTanImage", "ITKImageIntensity", "ImageIntensity"};
}

//------------------------------------------------------------------------------
Parameters ITKTanImage::parameters() const
{
  Parameters params;

  params.insertSeparator(Parameters::Separator{"Required Input Cell Data"});
  params.insert(std::make_unique<GeometrySelectionParameter>(k_SelectedImageGeomPath_Key, "Image Geometry", "Select the Image Geometry Group from the DataStructure.", DataPath({"Image Geometry"}),
                                                             GeometrySelectionParameter::AllowedTypes{IGeometry::Type::Image}));
  params.insert(std::make_unique<ArraySelectionParameter>(k_SelectedImageDataPath_Key, "Input Image Data Array", "The image data that will be processed by this filter.", DataPath{},
                                                          complex::ITK::GetScalarPixelAllowedTypes()));

  params.insertSeparator(Parameters::Separator{"Created Cell Data"});
  params.insert(
      std::make_unique<DataObjectNameParameter>(k_OutputImageDataPath_Key, "Output Image Data Array", "The result of the processing will be stored in this Data Array.", "Output Image Data"));

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer ITKTanImage::clone() const
{
  return std::make_unique<ITKTanImage>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult ITKTanImage::preflightImpl(const DataStructure& dataStructure, const Arguments& filterArgs, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const
{
  auto imageGeomPath = filterArgs.value<DataPath>(k_SelectedImageGeomPath_Key);
  auto selectedInputArray = filterArgs.value<DataPath>(k_SelectedImageDataPath_Key);
  auto outputArrayName = filterArgs.value<DataObjectNameParameter::ValueType>(k_OutputImageDataPath_Key);
  const DataPath outputArrayPath = selectedInputArray.getParent().createChildPath(outputArrayName);

  Result<OutputActions> resultOutputActions = ITK::DataCheck<cxITKTanImage::ArrayOptionsType>(dataStructure, selectedInputArray, imageGeomPath, outputArrayPath);

  return {std::move(resultOutputActions)};
}

//------------------------------------------------------------------------------
Result<> ITKTanImage::executeImpl(DataStructure& dataStructure, const Arguments& filterArgs, const PipelineFilter* pipelineNode, const MessageHandler& messageHandler,
                                  const std::atomic_bool& shouldCancel) const
{
  auto imageGeomPath = filterArgs.value<DataPath>(k_SelectedImageGeomPath_Key);
  auto selectedInputArray = filterArgs.value<DataPath>(k_SelectedImageDataPath_Key);
  auto outputArrayName = filterArgs.value<DataObjectNameParameter::ValueType>(k_OutputImageDataPath_Key);
  const DataPath outputArrayPath = selectedInputArray.getParent().createChildPath(outputArrayName);

  const cxITKTanImage::ITKTanImageFunctor itkFunctor = {};

  auto& imageGeom = dataStructure.getDataRefAs<ImageGeom>(imageGeomPath);
  imageGeom.getLinkedGeometryData().addCellData(outputArrayPath);

  return ITK::Execute<cxITKTanImage::ArrayOptionsType>(dataStructure, selectedInputArray, imageGeomPath, outputArrayPath, itkFunctor, shouldCancel);
}
} // namespace complex
