// Copyright (c) 2005-2019 Jay Berkenbilt
//
// This file is part of qpdf.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Versions of qpdf prior to version 7 were released under the terms
// of version 2.0 of the Artistic License. At your option, you may
// continue to consider qpdf to be licensed under those terms. Please
// see the manual for additional information.

#ifndef PL_DISCARD_HH
#define PL_DISCARD_HH

// This pipeline discards its output.  It is an end-of-line pipeline
// (with no next).

// This pipeline is reusable; i.e., it is safe to call write() after
// calling finish().

#include <qpdf/Pipeline.hh>

class Pl_Discard: public Pipeline
{
  public:
    QPDF_DLL
    Pl_Discard();
    QPDF_DLL
    virtual ~Pl_Discard();
    QPDF_DLL
    virtual void write(unsigned char*, size_t);
    QPDF_DLL
    virtual void finish();

  private:
    class Members
    {
        friend class Pl_Discard;

      public:
        QPDF_DLL
        ~Members();

      private:
        Members();
        Members(Members const&);
    };

    PointerHolder<Members> m;
};

#endif // PL_DISCARD_HH
