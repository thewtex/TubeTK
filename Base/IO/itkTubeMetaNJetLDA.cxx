/*=========================================================================

Library:   TubeTK

Copyright 2010 Kitware Inc. 28 Corporate Drive,
Clifton Park, NY, 12065, USA.

All rights reserved.

Licensed under the Apache License, Version 2.0 ( the "License" );
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

=========================================================================*/
#include "itkTubeMetaNJetLDA.h"

#include <stdio.h>
#include <ctype.h>
#include <string>
#include <string.h> // for memcpy
#include <math.h>

namespace itk {

namespace tube {

//
// MetaNJetLDA Constructors
//
MetaNJetLDA::
MetaNJetLDA()
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA()" << METAIO_STREAM::endl;
    }

  Clear();
}

//
MetaNJetLDA::
MetaNJetLDA( const char *_headerName )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA()" << METAIO_STREAM::endl;
    }

  Clear();
  m_ReadStream = NULL;

  MetaNJetLDA::Read( _headerName );
}

//
MetaNJetLDA::
MetaNJetLDA( const MetaNJetLDA & _metaNJetLDA )
{
  if( META_DEBUG )
   {
   METAIO_STREAM::cout << "MetaNJetLDA()" << METAIO_STREAM::endl;
   }

  Clear();

  CopyInfo( _metaNJetLDA );
}

//
MetaNJetLDA::
MetaNJetLDA(
  const NJetScalesType & _zeroScales,
  const NJetScalesType & _firstScales,
  const NJetScalesType & _secondScales,
  const NJetScalesType & _ridgeScales,
  const LDAValuesType & _ldaValues,
  const LDAMatrixType & _ldaMatrix )
{
  if( META_DEBUG )
   {
   METAIO_STREAM::cout << "MetaNJetLDA()" << METAIO_STREAM::endl;
   }

  Clear();

  InitializeEssential( _zeroScales, _firstScales, _secondScales,
    _ridgeScales, _ldaValues, _ldaMatrix );
}

//
MetaNJetLDA::
~MetaNJetLDA()
{
  M_Destroy();
}

//
void MetaNJetLDA::
PrintInfo() const
{
  MetaLDA::PrintInfo();

  METAIO_STREAM::cout << "ZeroScales = " << m_ZeroScales.size()
    << METAIO_STREAM::endl;
  METAIO_STREAM::cout << "FirstScales = " << m_FirstScales.size()
    << METAIO_STREAM::endl;
  METAIO_STREAM::cout << "SecondScales = " << m_SecondScales.size()
    << METAIO_STREAM::endl;
  METAIO_STREAM::cout << "RidgeScales = " << m_RidgeScales.size()
    << METAIO_STREAM::endl;
}

void MetaNJetLDA::
CopyInfo( const MetaNJetLDA & _lda )
{
  MetaLDA::CopyInfo( dynamic_cast< const MetaLDA & >( _lda ) );

  SetZeroScales( _lda.GetZeroScales() );
  SetFirstScales( _lda.GetFirstScales() );
  SetSecondScales( _lda.GetSecondScales() );
  SetRidgeScales( _lda.GetSecondScales() );
}

void MetaNJetLDA::
Clear( void )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: Clear" << METAIO_STREAM::endl;
    }

  m_ZeroScales.resize( 0 );
  m_FirstScales.resize( 0 );
  m_SecondScales.resize( 0 );
  m_RidgeScales.resize( 0 );

  MetaLDA::Clear();
}

bool MetaNJetLDA::
InitializeEssential(
  const NJetScalesType & _zeroScales,
  const NJetScalesType & _firstScales,
  const NJetScalesType & _secondScales,
  const NJetScalesType & _ridgeScales,
  const LDAValuesType & _ldaValues,
  const LDAMatrixType & _ldaMatrix )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: Initialize" << METAIO_STREAM::endl;
    }

  MetaLDA::InitializeEssential( _ldaValues, _ldaMatrix );

  SetZeroScales( _zeroScales );
  SetFirstScales( _firstScales );
  SetSecondScales( _secondScales );
  SetRidgeScales( _ridgeScales );

  return true;
}

//
//
//
void MetaNJetLDA::
SetZeroScales( const NJetScalesType & _ZeroScales )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: SetZeroScales"
      << METAIO_STREAM::endl;
    }

  m_ZeroScales = _ZeroScales;
}

const MetaNJetLDA::NJetScalesType & MetaNJetLDA::
GetZeroScales( void ) const
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: GetZeroScales"
      << METAIO_STREAM::endl;
    }

  return m_ZeroScales;
}

//
//
//
void MetaNJetLDA::
SetFirstScales( const NJetScalesType & _FirstScales )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: SetFirstScales"
      << METAIO_STREAM::endl;
    }

  m_FirstScales = _FirstScales;
}

const MetaNJetLDA::NJetScalesType & MetaNJetLDA::
GetFirstScales( void ) const
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: GetFirstScales"
      << METAIO_STREAM::endl;
    }

  return m_FirstScales;
}

//
//
//
void MetaNJetLDA::
SetSecondScales( const NJetScalesType & _SecondScales )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: SetSecondScales"
      << METAIO_STREAM::endl;
    }

  m_SecondScales = _SecondScales;
}

const MetaNJetLDA::NJetScalesType & MetaNJetLDA::
GetSecondScales( void ) const
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: GetSecondScales"
      << METAIO_STREAM::endl;
    }

  return m_SecondScales;
}

//
//
//
void MetaNJetLDA::
SetRidgeScales( const NJetScalesType & _RidgeScales )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: SetRidgeScales"
      << METAIO_STREAM::endl;
    }

  m_RidgeScales = _RidgeScales;
}

const MetaNJetLDA::NJetScalesType & MetaNJetLDA::
GetRidgeScales( void ) const
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: GetRidgeScales"
      << METAIO_STREAM::endl;
    }

  return m_RidgeScales;
}

//
//
//
bool MetaNJetLDA::
CanRead( const char *_headerName ) const
{
  // First check the extension
  METAIO_STL::string fname = _headerName;
  if( fname == "" )
    {
    return false;
    }

  bool extensionFound = false;

  METAIO_STL::string::size_type stringPos = fname.rfind( ".mnda" );
  if ( ( stringPos != METAIO_STL::string::npos )
      && ( stringPos == fname.length() - 5 ) )
    {
    extensionFound = true;
    }

  if( !extensionFound )
    {
    return false;
    }

  // Now check the file content
  METAIO_STREAM::ifstream inputStream;

  inputStream.open( _headerName, METAIO_STREAM::ios::in |
                                 METAIO_STREAM::ios::binary );

  if( !inputStream.rdbuf()->is_open() )
    {
    return false;
    }

  bool result = !strncmp( MET_ReadForm( inputStream ).c_str(),
    "NJetLDA", 7 );

  inputStream.close();

  return result;
}


bool MetaNJetLDA::
Read( const char *_headerName )
{
  if( _headerName != NULL && strlen( _headerName ) > 1 )
    {
    FileName( _headerName );
    }

  METAIO_STREAM::ifstream * tmpStream = new METAIO_STREAM::ifstream;

  tmpStream->open( m_FileName, METAIO_STREAM::ios::in |
                               METAIO_STREAM::ios::binary );

  if( !tmpStream->rdbuf()->is_open() )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: Read: Cannot open file _"
                        << m_FileName << "_" << METAIO_STREAM::endl;
    delete tmpStream;
    return false;
    }

  bool result = ReadStream( tmpStream );

  tmpStream->close();

  delete tmpStream;

  return result;
}


bool MetaNJetLDA::
CanReadStream( METAIO_STREAM::ifstream * _stream ) const
{
  if( !strncmp( MET_ReadForm( *_stream ).c_str(), "NJetLDA", 7 ) )
    {
    return true;
    }

  return false;
}

bool MetaNJetLDA::
ReadStream( METAIO_STREAM::ifstream * _stream )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: ReadStream" << METAIO_STREAM::endl;
    }

  M_Destroy();

  Clear();

  M_SetupReadFields();

  if( m_ReadStream )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: ReadStream: two files open?"
                        << METAIO_STREAM::endl;
    delete m_ReadStream;
    }

  m_ReadStream = _stream;

  if( !M_Read() )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: Read: Cannot parse file"
                        << METAIO_STREAM::endl;
    m_ReadStream = NULL;
    return false;
    }

  m_ReadStream = NULL;

  InitializeEssential( m_ZeroScales, m_FirstScales, m_SecondScales,
    m_RidgeScales, m_LDAValues, m_LDAMatrix );

  return true;
}

//
//
//
//

bool MetaNJetLDA::
Write( const char *_headName )
{
  if( _headName != NULL && strlen( _headName ) > 1 )
    {
    FileName( _headName );
    }

  MET_SetFileSuffix( m_FileName, "mnda" );

  METAIO_STREAM::ofstream * tmpWriteStream = new METAIO_STREAM::ofstream;

  tmpWriteStream->open( m_FileName, METAIO_STREAM::ios::binary |
                                   METAIO_STREAM::ios::out );

  if( !tmpWriteStream->rdbuf()->is_open() )
    {
    delete tmpWriteStream;
    return false;
    }

  bool result = WriteStream( tmpWriteStream );

  tmpWriteStream->close();

  delete tmpWriteStream;

  return result;
}

bool MetaNJetLDA::
WriteStream( METAIO_STREAM::ofstream * _stream )
{
  if( m_WriteStream != NULL )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: WriteStream: two files open?"
                        << METAIO_STREAM::endl;
    delete m_WriteStream;
    }

  m_WriteStream = _stream;

  M_SetupWriteFields();

  M_Write();

  m_WriteStream->flush();

  m_WriteStream = NULL;

  return true;
}

void MetaNJetLDA::
M_Destroy( void )
{
  MetaLDA::M_Destroy();
}

void MetaNJetLDA::
M_SetupReadFields( void )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: M_SetupReadFields"
                        << METAIO_STREAM::endl;
    }

  MetaLDA::M_SetupReadFields();

  MET_FieldRecordType * mF;

  mF = new MET_FieldRecordType;
  MET_InitReadField( mF, "NZeroScales", MET_INT, false );
  m_Fields.push_back( mF );
  int nScalesRecNum = MET_GetFieldRecordNumber( "NZeroScales", &m_Fields );
  mF = new MET_FieldRecordType;
  MET_InitReadField( mF, "ZeroScales", MET_FLOAT_ARRAY, false,
    nScalesRecNum );
  m_Fields.push_back( mF );

  mF = new MET_FieldRecordType;
  MET_InitReadField( mF, "NFirstScales", MET_INT, false );
  m_Fields.push_back( mF );
  nScalesRecNum = MET_GetFieldRecordNumber( "NFirstScales", &m_Fields );
  mF = new MET_FieldRecordType;
  MET_InitReadField( mF, "FirstScales", MET_FLOAT_ARRAY, false,
    nScalesRecNum );
  m_Fields.push_back( mF );

  mF = new MET_FieldRecordType;
  MET_InitReadField( mF, "NSecondScales", MET_INT, false );
  m_Fields.push_back( mF );
  nScalesRecNum = MET_GetFieldRecordNumber( "NSecondScales", &m_Fields );
  mF = new MET_FieldRecordType;
  MET_InitReadField( mF, "SecondScales", MET_FLOAT_ARRAY, false,
    nScalesRecNum );
  m_Fields.push_back( mF );

  mF = new MET_FieldRecordType;
  MET_InitReadField( mF, "NRidgeScales", MET_INT, false );
  m_Fields.push_back( mF );
  nScalesRecNum = MET_GetFieldRecordNumber( "NRidgeScales", &m_Fields );
  mF = new MET_FieldRecordType;
  MET_InitReadField( mF, "RidgeScales", MET_FLOAT_ARRAY, false,
    nScalesRecNum );
  m_Fields.push_back( mF );
}

void MetaNJetLDA::
M_SetupWriteFields( void )
{
  strcpy( m_FormTypeName, "NJetLDA" );
  MetaLDA::M_SetupWriteFields();

  MET_FieldRecordType * mF;

  if( m_ZeroScales.size() > 0 )
    {
    m_ZeroScalesTmp.set_size( m_ZeroScales.size() );
    for( unsigned int i=0; i<m_ZeroScales.size(); i++ )
      {
      m_ZeroScalesTmp[i] = m_ZeroScales[i];
      }
    mF = new MET_FieldRecordType;
    MET_InitWriteField( mF, "NZeroScales", MET_INT, m_ZeroScales.size() );
    m_Fields.push_back( mF );
    int nZeroScales = m_ZeroScales.size();
    mF = new MET_FieldRecordType;
    MET_InitWriteField( mF, "ZeroScales", MET_FLOAT_ARRAY, nZeroScales,
      m_ZeroScalesTmp.data_block() );
    m_Fields.push_back( mF );
    }

  if( m_FirstScales.size() > 0 )
    {
    m_FirstScalesTmp.set_size( m_FirstScales.size() );
    for( unsigned int i=0; i<m_FirstScales.size(); i++ )
      {
      m_FirstScalesTmp[i] = m_FirstScales[i];
      }
    mF = new MET_FieldRecordType;
    MET_InitWriteField( mF, "NFirstScales", MET_INT, m_FirstScales.size() );
    m_Fields.push_back( mF );
    int nFirstScales = m_FirstScales.size();
    mF = new MET_FieldRecordType;
    MET_InitWriteField( mF, "FirstScales", MET_FLOAT_ARRAY, nFirstScales,
      m_FirstScalesTmp.data_block() );
    m_Fields.push_back( mF );
    }

  if( m_SecondScales.size() > 0 )
    {
    m_SecondScalesTmp.set_size( m_SecondScales.size() );
    for( unsigned int i=0; i<m_SecondScales.size(); i++ )
      {
      m_SecondScalesTmp[i] = m_SecondScales[i];
      }
    mF = new MET_FieldRecordType;
    MET_InitWriteField( mF, "NSecondScales", MET_INT,
      m_SecondScales.size() );
    m_Fields.push_back( mF );
    int nSecondScales = m_SecondScales.size();
    mF = new MET_FieldRecordType;
    MET_InitWriteField( mF, "SecondScales", MET_FLOAT_ARRAY, nSecondScales,
      m_SecondScalesTmp.data_block() );
    m_Fields.push_back( mF );
    }

  if( m_RidgeScales.size() > 0 )
    {
    m_RidgeScalesTmp.set_size( m_RidgeScales.size() );
    for( unsigned int i=0; i<m_RidgeScales.size(); i++ )
      {
      m_RidgeScalesTmp[i] = m_RidgeScales[i];
      }
    mF = new MET_FieldRecordType;
    MET_InitWriteField( mF, "NRidgeScales", MET_INT, m_RidgeScales.size() );
    m_Fields.push_back( mF );
    int nRidgeScales = m_RidgeScales.size();
    mF = new MET_FieldRecordType;
    MET_InitWriteField( mF, "RidgeScales", MET_FLOAT_ARRAY, nRidgeScales,
      m_RidgeScalesTmp.data_block() );
    m_Fields.push_back( mF );
    }

}

bool MetaNJetLDA::
M_Read( void )
{
  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: M_Read: Loading Header"
                        << METAIO_STREAM::endl;
    }
  if( !MetaLDA::M_Read() )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: M_Read: Error parsing file"
                        << METAIO_STREAM::endl;
    return false;
    }

  if( META_DEBUG )
    {
    METAIO_STREAM::cout << "MetaNJetLDA: M_Read: Parsing Header"
                        << METAIO_STREAM::endl;
    }
  MET_FieldRecordType * mF;

  unsigned int nZeroScales = 0;
  mF = MET_GetFieldRecord( "NZeroScales", &m_Fields );
  if( mF && mF->defined )
    {
    nZeroScales = ( unsigned int )mF->value[0];
    m_ZeroScales.resize( nZeroScales, 0 );
    mF = MET_GetFieldRecord( "ZeroScales", &m_Fields );
    if( mF && mF->defined )
      {
      for( unsigned int i=0; i<nZeroScales; i++ )
        {
        m_ZeroScales[i] = ( double )mF->value[i];
        }
      }
    }
  else
    {
    m_ZeroScales.clear();
    }

  unsigned int nFirstScales = 0;
  mF = MET_GetFieldRecord( "NFirstScales", &m_Fields );
  if( mF && mF->defined )
    {
    nFirstScales = ( unsigned int )mF->value[0];
    m_FirstScales.resize( nFirstScales, 0 );
    mF = MET_GetFieldRecord( "FirstScales", &m_Fields );
    if( mF && mF->defined )
      {
      for( unsigned int i=0; i<nFirstScales; i++ )
        {
        m_FirstScales[i] = ( double )mF->value[i];
        }
      }
    }
  else
    {
    m_FirstScales.clear();
    }

  unsigned int nSecondScales = 0;
  mF = MET_GetFieldRecord( "NSecondScales", &m_Fields );
  if( mF && mF->defined )
    {
    nSecondScales = ( unsigned int )mF->value[0];
    m_SecondScales.resize( nSecondScales, 0 );
    mF = MET_GetFieldRecord( "SecondScales", &m_Fields );
    if( mF && mF->defined )
      {
      for( unsigned int i=0; i<nSecondScales; i++ )
        {
        m_SecondScales[i] = ( double )mF->value[i];
        }
      }
    }
  else
    {
    m_SecondScales.clear();
    }

  unsigned int nRidgeScales = 0;
  mF = MET_GetFieldRecord( "NRidgeScales", &m_Fields );
  if( mF && mF->defined )
    {
    nRidgeScales = ( unsigned int )mF->value[0];
    m_RidgeScales.resize( nRidgeScales, 0 );
    mF = MET_GetFieldRecord( "RidgeScales", &m_Fields );
    if( mF && mF->defined )
      {
      for( unsigned int i=0; i<nRidgeScales; i++ )
        {
        m_RidgeScales[i] = ( double )mF->value[i];
        }
      }
    }
  else
    {
    m_RidgeScales.clear();
    }

  return true;
}

}

}
