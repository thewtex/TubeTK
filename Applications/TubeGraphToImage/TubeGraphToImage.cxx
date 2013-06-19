/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/

#include "tubeCLIFilterWatcher.h"
#include "tubeCLIProgressReporter.h"
#include "tubeMessage.h"

#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkImageRegionIterator.h>
#include <itkMatrix.h>
#include <itkTimeProbesCollectorBase.h>

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

#include "TubeGraphToImageCLP.h"

template< class pixelT, unsigned int dimensionT >
int DoIt( int argc, char * argv[] );

// Must follow include of "...CLP.h" and forward declaration of int DoIt( ... ).
#include "tubeCLIHelperFunctions.h"

template< class pixelT, unsigned int dimensionT >
int DoIt( int argc, char * argv[] )
{
  PARSE_ARGS;

  std::stringstream logMsg;

  typedef itk::Image< pixelT, dimensionT >      CVTImageType;
  typedef itk::ImageFileReader< CVTImageType >  ImageReaderType;

  typename ImageReaderType::Pointer imageReader = ImageReaderType::New();

  // Read image file generated by CVT
  std::string cvtImageFilename = inCentroidsFile + ".mha";
  imageReader->SetFileName( cvtImageFilename.c_str() );
  imageReader->Update();
  typename CVTImageType::Pointer cvtImage = imageReader->GetOutput();

  double tf;
  int numberOfCentroids;
  int numberOfCentroids2;
  std::ifstream readStream;

  // Read full connectivity information
  std::string matrixFilename = inGraphFile + ".mat";

  logMsg.str( "" );
  logMsg << "Reading file: " << matrixFilename;
  tube::InfoMessage( logMsg.str() );

  readStream.open(matrixFilename.c_str(), std::ios::in);
  readStream >> numberOfCentroids;
  readStream.get();

  logMsg.str( "" );
  logMsg << "Number of centroids: " << numberOfCentroids;
  tube::InfoMessage( logMsg.str() );

  vnl_matrix<double> aMat(numberOfCentroids, numberOfCentroids);
  aMat.fill(0);
  vnl_vector<double> bVect(numberOfCentroids);
  bVect.fill(0);
  vnl_vector<double> rVect(numberOfCentroids);
  rVect.fill(0);
  vnl_vector<double> cVect(numberOfCentroids);
  cVect.fill(0);
  for(int i=0; i<numberOfCentroids; i++)
    {
    for(int j=0; j<numberOfCentroids; j++)
      {
      readStream >> tf;
      readStream.get();
      aMat[i][j] = tf;
      }
    }
  readStream.close();

  // Read in BRANCH file
  std::string branchFilename = inGraphFile + ".brc";

  logMsg.str( "" );
  logMsg << "Reading (branch) file: " << branchFilename;
  tube::InfoMessage( logMsg.str() );

  readStream.open(branchFilename.c_str(), std::ios::in);
  readStream >> numberOfCentroids2;
  readStream.get();
  if(numberOfCentroids != numberOfCentroids2)
    {
    tube::ErrorMessage( "Error: fileList's #Centroids != branch #Centroids" );
    return EXIT_FAILURE;
    }
  for(int i=0; i<numberOfCentroids; i++)
    {
    readStream >> tf;
    readStream.get();
    bVect[i] = tf;
    }
  readStream.close();

  // Read in ROOT file
  std::string rootFilename = inGraphFile + ".rot";

  logMsg.str( "" );
  logMsg << "Reading (root) file: " << rootFilename;
  tube::InfoMessage( logMsg.str() );

  readStream.open(rootFilename.c_str(), std::ios::in);
  readStream >> numberOfCentroids2;
  readStream.get();
  if(numberOfCentroids != numberOfCentroids2)
    {
    tube::ErrorMessage( "Error: fileList's #Centroids != root #Centroids" );
    return EXIT_FAILURE;
    }
  for(int i=0; i<numberOfCentroids; i++)
    {
    readStream >> tf;
    readStream.get();
    rVect[i] = tf;
    }
  readStream.close();

  // Read in CENTRALITY file
  std::string centralityFilename = inGraphFile + ".cnt";

  logMsg.str( "" );
  logMsg << "Reading (centrality) file: " << centralityFilename;
  tube::InfoMessage( logMsg.str() );

  readStream.open(centralityFilename.c_str(), std::ios::in);
  readStream >> numberOfCentroids2;
  readStream.get();
  if(numberOfCentroids != numberOfCentroids2)
    {
    tube::ErrorMessage( "Error: fileList's #Centroids != centrality #Centroids" );
    return EXIT_FAILURE;
    }
  for(int i=0; i<numberOfCentroids; i++)
    {
    readStream >> tf;
    readStream.get();
    cVect[i] = tf;
    }
  readStream.close();

  typedef itk::Image< float, dimensionT > ImageType;

  typename ImageType::Pointer aImage = ImageType::New();
  aImage->SetRegions( cvtImage->GetLargestPossibleRegion().GetSize() );
  aImage->SetSpacing( cvtImage->GetSpacing() );
  aImage->SetOrigin( cvtImage->GetOrigin() );
  aImage->Allocate();

  typename ImageType::Pointer bImage = ImageType::New();
  bImage->SetRegions( cvtImage->GetLargestPossibleRegion().GetSize() );
  bImage->SetSpacing( cvtImage->GetSpacing() );
  bImage->SetOrigin( cvtImage->GetOrigin() );
  bImage->Allocate();

  typename ImageType::Pointer rImage = ImageType::New();
  rImage->SetRegions( cvtImage->GetLargestPossibleRegion().GetSize() );
  rImage->SetSpacing( cvtImage->GetSpacing() );
  rImage->SetOrigin( cvtImage->GetOrigin() );
  rImage->Allocate();

  typename ImageType::Pointer cImage = ImageType::New();
  cImage->SetRegions( cvtImage->GetLargestPossibleRegion().GetSize() );
  cImage->SetSpacing( cvtImage->GetSpacing() );
  cImage->SetOrigin( cvtImage->GetOrigin() );
  cImage->Allocate();

  itk::ImageRegionIterator< CVTImageType >
                 itCVT( cvtImage, cvtImage->GetLargestPossibleRegion() );
  itk::ImageRegionIterator< ImageType >
                 itA( aImage, aImage->GetLargestPossibleRegion() );
  itk::ImageRegionIterator< ImageType >
                 itB( bImage, bImage->GetLargestPossibleRegion() );
  itk::ImageRegionIterator< ImageType >
                 itR( rImage, rImage->GetLargestPossibleRegion() );
  itk::ImageRegionIterator< ImageType >
                 itC( cImage, cImage->GetLargestPossibleRegion() );
  itCVT.GoToBegin();
  itA.GoToBegin();
  itB.GoToBegin();
  itR.GoToBegin();
  itC.GoToBegin();
  pixelT c;
  while( !itCVT.IsAtEnd() )
    {
    c = itCVT.Get()-1;
    itB.Set( bVect[c] );
    itR.Set( rVect[c] );
    itC.Set( cVect[c] );
    double maxT = 0;
    for( int i=0; i<numberOfCentroids; i++ )
      {
      if( aMat[c][i] > maxT )
        {
        maxT = aMat[c][i];
        }
      }
    itA.Set( maxT );
    ++itCVT;
    ++itA;
    ++itB;
    ++itR;
    ++itC;
    }

  typedef itk::ImageFileWriter< ImageType > ImageWriterType;
  typename ImageWriterType::Pointer imageWriter;
  std::string filename;

  filename = outImageFile + ".mat.mha";
  imageWriter = ImageWriterType::New();
  imageWriter->SetFileName( filename.c_str() );
  imageWriter->SetInput( aImage );
  imageWriter->Write();

  filename = outImageFile + ".brc.mha";
  imageWriter = ImageWriterType::New();
  imageWriter->SetFileName( filename.c_str() );
  imageWriter->SetInput( bImage );
  imageWriter->Write();

  filename = outImageFile + ".rot.mha";
  imageWriter = ImageWriterType::New();
  imageWriter->SetFileName( filename.c_str() );
  imageWriter->SetInput( rImage );
  imageWriter->Write();

  filename = outImageFile + ".cnt.mha";
  imageWriter = ImageWriterType::New();
  imageWriter->SetFileName( filename.c_str() );
  imageWriter->SetInput( cImage );
  imageWriter->Write();

  return EXIT_SUCCESS;
}

int main( int argc, char * argv[] )
{
  PARSE_ARGS;

  return tube::ParseArgsAndCallDoIt( inCentroidsFile, argc, argv );
}
