/* ============================================================================
 * Copyright (c) 2011 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2011 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "WriteFieldData.h"

#include "DREAM3DLib/Common/DREAM3DMath.h"
#include "DREAM3DLib/Common/Constants.h"
#include "DREAM3DLib/Common/DREAM3DRandom.h"


const static float m_pi = static_cast<float>(M_PI);


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
WriteFieldData::WriteFieldData() :
AbstractFilter(),
m_Phases(NULL),
m_EulerAngles(NULL),
m_EquivalentDiameters(NULL),
m_AspectRatios(NULL),
m_Omega3s(NULL),
m_SurfaceFields(NULL),
m_UnbiasedFields(NULL)
{
  setupFilterOptions();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
WriteFieldData::~WriteFieldData()
{
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void WriteFieldData::setupFilterOptions()
{
  std::vector<FilterOption::Pointer> options;
  {
    FilterOption::Pointer option = FilterOption::New();
    option->setHumanLabel("Output File");
    option->setPropertyName("FieldDataFile");
    option->setWidgetType(FilterOption::OutputFileWidget);
    option->setValueType("string");
    options.push_back(option);
  }
  setFilterOptions(options);
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void WriteFieldData::preflight()
{
  int err = 0;
  std::stringstream ss;
  DataContainer::Pointer m = DataContainer::New();

  setErrorCondition(err);
  setErrorMessage(ss.str());
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void WriteFieldData::execute()
{
  setErrorCondition(0);
  std::string filename = getFieldDataFile();
  DataContainer* m = getDataContainer();
  std::ofstream outFile;
  outFile.open(filename.c_str(), std::ios_base::binary);
  char space = DREAM3D::GrainData::Delimiter;
  outFile << m->getTotalFields()-1 << std::endl;
  outFile << DREAM3D::GrainData::GrainID  << space << DREAM3D::GrainData::PhaseID << space
      << DREAM3D::GrainData::Phi1 << space << DREAM3D::GrainData::PHI<< space << DREAM3D::GrainData::Phi2 << space
      << DREAM3D::GrainData::EquivDiam << space
	  << DREAM3D::GrainData::B_Over_A << space << DREAM3D::GrainData::C_Over_A << space << DREAM3D::GrainData::Omega3 << space << DREAM3D::GrainData::SurfaceGrain << space << DREAM3D::GrainData::OutsideBoundingBox << std::endl;

  for (size_t i = 1; i < m->getTotalFields(); i++)
  {
	  outFile << i << space << m_Phases[i] << space << m_EulerAngles[3*i] << space << m_EulerAngles[3*i+1] << space << m_EulerAngles[3*i+2] <<
		space << m_EquivalentDiameters[i] << space << m_AspectRatios[2*i] << space << m_AspectRatios[2*i+1] <<
		space << m_Omega3s[i] << space << int(m_SurfaceFields[i]) << space << int(m_UnbiasedFields[i]) << std::endl;
  }
  outFile.close();

  // If there is an error set this to something negative and also set a message
  notify("WriteFieldData Completed", 0, Observable::UpdateProgressMessage);

}







