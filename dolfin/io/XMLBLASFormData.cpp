// Copyright (C) 2005-2006 Anders Logg.
// Licensed under the GNU LGPL Version 2.1.
//
// First added:  2005-10-02
// Last changed: 2006-05-23

#include <dolfin/log/dolfin_log.h>
#include "XMLBLASFormData.h"

using namespace dolfin;

//-----------------------------------------------------------------------------
XMLBLASFormData::XMLBLASFormData(BLASFormData& blas) :
  XMLObject(), blas(blas), mi(0), ni(0), mb(0), nb(0), state(OUTSIDE)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
void XMLBLASFormData::start_element(const xmlChar *name, const xmlChar **attrs)
{
  //dolfin_debug1("Found start of element \"%s\"", (const char *) name);

  switch ( state )
  {
  case OUTSIDE:

    if ( xmlStrcasecmp(name,(xmlChar *) "form") == 0 )
    {
      read_form(name, attrs);
      state = INSIDE_FORM;
    }

    break;

  case INSIDE_FORM:

    if ( xmlStrcasecmp(name,(xmlChar *) "interior") == 0 )
    {
      read_interior(name, attrs);
      state = INSIDE_INTERIOR;
    }
    else if ( xmlStrcasecmp(name,(xmlChar *) "boundary") == 0 )
    {
      read_boundary(name, attrs);
      state = INSIDE_BOUNDARY;
    }

    break;

  case INSIDE_INTERIOR:

    if ( xmlStrcasecmp(name,(xmlChar *) "term") == 0 )
    {
      read_term(name,attrs);
      state = INSIDE_INTERIOR_TERM;
    }

    break;

  case INSIDE_BOUNDARY:

    if ( xmlStrcasecmp(name,(xmlChar *) "term") == 0 )
    {
      read_term(name,attrs);
      state = INSIDE_BOUNDARY_TERM;
    }

    break;

  case INSIDE_INTERIOR_TERM:

    if ( xmlStrcasecmp(name,(xmlChar *) "geometrytensor") == 0 )
    {
      readGeoTensor(name, attrs);
      state = INSIDE_INTERIOR_GEOTENSOR;
    }
    else if ( xmlStrcasecmp(name,(xmlChar *) "referencetensor") == 0 )
    {
      readRefTensor(name, attrs);
      state = INSIDE_INTERIOR_REFTENSOR;
    }

    break;

  case INSIDE_BOUNDARY_TERM:

    if ( xmlStrcasecmp(name,(xmlChar *) "referencetensor") == 0 )
    {
      readRefTensor(name, attrs);
      state = INSIDE_BOUNDARY_REFTENSOR;
    }

    break;

  case INSIDE_INTERIOR_GEOTENSOR:
    break;

  case INSIDE_BOUNDARY_GEOTENSOR:
    break;

  case INSIDE_INTERIOR_REFTENSOR:

    if ( xmlStrcasecmp(name,(xmlChar *) "entry") == 0 )
    {
      read_entry(name, attrs);
    }

    break;

  case INSIDE_BOUNDARY_REFTENSOR:

    if ( xmlStrcasecmp(name,(xmlChar *) "entry") == 0 )
    {
      read_entry(name, attrs);
    }

    break;

  default:
    ;
  }

}
//-----------------------------------------------------------------------------
void XMLBLASFormData::end_element(const xmlChar *name)
{
  //dolfin_debug1("Found end of element \"%s\"", (const char *) name);

  switch ( state )
  {
  case INSIDE_FORM:

    if ( xmlStrcasecmp(name,(xmlChar *) "form") == 0 )
    {
      init_form();
      data_interior.clear();
      data_interior.clear();
      state = DONE;
    }

    break;

  case INSIDE_INTERIOR:

    if ( xmlStrcasecmp(name,(xmlChar *) "interior") == 0 )
      state = INSIDE_FORM;

    break;

  case INSIDE_BOUNDARY:

    if ( xmlStrcasecmp(name,(xmlChar *) "boundary") == 0 )
      state = INSIDE_FORM;

    break;

  case INSIDE_INTERIOR_TERM:

    if ( xmlStrcasecmp(name,(xmlChar *) "term") == 0 )
      state = INSIDE_INTERIOR;

    break;

  case INSIDE_BOUNDARY_TERM:

    if ( xmlStrcasecmp(name,(xmlChar *) "term") == 0 )
      state = INSIDE_BOUNDARY;

    break;

  case INSIDE_INTERIOR_GEOTENSOR:

    if ( xmlStrcasecmp(name,(xmlChar *) "geometrytensor") == 0 )
      state = INSIDE_INTERIOR_TERM;

    break;

  case INSIDE_BOUNDARY_GEOTENSOR:

    if ( xmlStrcasecmp(name,(xmlChar *) "geometrytensor") == 0 )
      state = INSIDE_BOUNDARY_TERM;

    break;

  case INSIDE_INTERIOR_REFTENSOR:

    if ( xmlStrcasecmp(name,(xmlChar *) "referencetensor") == 0 )
      state = INSIDE_INTERIOR_TERM;

    break;

  case INSIDE_BOUNDARY_REFTENSOR:

    if ( xmlStrcasecmp(name,(xmlChar *) "referencetensor") == 0 )
      state = INSIDE_BOUNDARY_TERM;

    break;

  default:
    ;
  }

}
//-----------------------------------------------------------------------------
void XMLBLASFormData::open(std::string filename)
{
  message(1, "Reading form data from file \"%s\".", filename.c_str());
}
//-----------------------------------------------------------------------------
bool XMLBLASFormData::close()
{
  return true;
}
//-----------------------------------------------------------------------------
void XMLBLASFormData::read_form(const xmlChar *name, const xmlChar **attrs)
{
  // Reset data
  data_interior.clear();
  data_boundary.clear();
}
//-----------------------------------------------------------------------------
void XMLBLASFormData::read_interior(const xmlChar *name, const xmlChar **attrs)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
void XMLBLASFormData::read_boundary(const xmlChar *name, const xmlChar **attrs)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
void XMLBLASFormData::read_term(const xmlChar *name, const xmlChar **attrs)
{
  std::vector<double> tensor;

  // Add new term and read number of rows
  switch ( state )
  {
  case INSIDE_INTERIOR:
    data_interior.push_back(tensor);
    mi = parse_int(name, attrs, "size");
    break;
  case INSIDE_BOUNDARY:
    data_boundary.push_back(tensor);
    mb = parse_int(name, attrs, "size");
    break;
  default:
    cout << state << endl;
    error("Inconsistent state while reading XML form data.");
  }

}
//-----------------------------------------------------------------------------
void XMLBLASFormData::readGeoTensor(const xmlChar *name, const xmlChar **attrs)
{
  // Read number of columns
  switch ( state )
  {
  case INSIDE_INTERIOR_TERM:
    ni = parse_int(name, attrs, "size");
    break;
  case INSIDE_BOUNDARY_TERM:
    nb = parse_int(name, attrs, "size");
    break;
  default:
    error("Inconsistent state while reading XML form data.");
  }
}
//-----------------------------------------------------------------------------
void XMLBLASFormData::readRefTensor(const xmlChar *name, const xmlChar **attrs)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
void XMLBLASFormData::read_entry(const xmlChar *name, const xmlChar **attrs)
{
  // Read value
  double value = parse_real(name, attrs, "value");

  // Append value to tensor
  switch ( state )
  {
  case INSIDE_INTERIOR_REFTENSOR:
    data_interior.back().push_back(value);
    break;
  case INSIDE_BOUNDARY_REFTENSOR:
    data_boundary.back().push_back(value);
    break;
  default:
    error("Inconsistent state while reading XML data.");
  }
}
//-----------------------------------------------------------------------------
void XMLBLASFormData::init_form()
{
  error("XMLBLASFormData not implemented for new UFC strcuture.");
/*
  // Give data to form
  blas.init(mi, ni, data_interior, mb, nb, data_boundary);
*/
}
//-----------------------------------------------------------------------------
