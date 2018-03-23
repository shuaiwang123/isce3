//-*- C++ -*-
//-*- coding: utf-8 -*-
//
// Author: ml
// Original code: Joshua Cohen
// Copyright 2018
//

#ifndef __ISCE_CORE_RASTER_H__
#define __ISCE_CORE_RASTER_H__

#include <complex>
#include <cstdint>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include "gdal_priv.h"

namespace isce {
  namespace core {
    
    class Raster {
      
    public:
      
      /* Constructors and destructors */
      Raster(const std::string&, bool);
      Raster(const std::string&, size_t, size_t, size_t, GDALDataType, const std::string&);
      Raster(const std::string&, size_t, size_t, size_t, GDALDataType);
      Raster(const std::string&, size_t, size_t, size_t);
      Raster(const std::string&, size_t, size_t, GDALDataType);
      Raster(const std::string&, size_t, size_t);
      Raster(const std::string&, const Raster&);
      template<typename T> Raster(const std::string&, const std::vector<T>&, size_t); 
      Raster(const Raster&);
      ~Raster();

      inline Raster& operator=(const Raster&);

      inline GDALDataset* dataset()  const { return _dataset; }
      inline size_t       length()   const { return _dataset->GetRasterYSize(); }
      inline size_t       width()    const { return _dataset->GetRasterXSize(); }
      inline size_t       numBands() const { return _dataset->GetRasterCount(); }
      inline GDALDataType dtype()    const { return _dataset->GetRasterBand(1)->GetRasterDataType(); }


      void load(const std::string&,bool);
      
      /* Pixel operations */

      // (buffer, x-index, y-index, band-index, read/write)
      template<typename T> void getSetValue(T&,size_t,size_t,size_t,bool);

      // (buffer, x-index, y-index, [band-index])
      template<typename T> void getValue(T&,size_t,size_t,size_t);
      template<typename T> void getValue(T&,size_t,size_t);

      // (buffer, x-index, y-index, [band-index])
      template<typename T> void setValue(T&,size_t,size_t,size_t);
      template<typename T> void setValue(T&,size_t,size_t);

      /* Line operations */
      // (buffer, line-index, nelem, band-index, read/write)
      template<typename T> void getSetLine(T*,size_t,size_t,size_t,bool);

      // Single line read, non-specific container w/ container width, optional band index
      // (buffer, line-index, nelem, [band-index])
      template<typename T> void getLine(T*,size_t,size_t,size_t);
      template<typename T> void getLine(T*,size_t,size_t);

      // Single line read, STL containers, optional band index
      // (buffer, line-index, [band-index])
      template<typename T> void getLine(std::vector<T>&,size_t,size_t);
      template<typename T> void getLine(std::vector<T>&,size_t);

      // Single line write, non-specific container w/ container width, optional band index
      // (buffer, line-index, nelem, [band-index])
      template<typename T> void setLine(T*,size_t,size_t,size_t);
      template<typename T> void setLine(T*,size_t,size_t);

      // Single line write, STL containers, optional band index
      // (buffer, line-index, [band-index])
      template<typename T> void setLine(std::vector<T>&,size_t,size_t);
      template<typename T> void setLine(std::vector<T>&,size_t);

      /* Block operations */
      // (buffer, x-index, y-index, nXelem, nYelem, band-index, read/write)
      template<typename T> void getSetBlock(T*,size_t,size_t,size_t,size_t,size_t,bool);

      // (buffer, x-index, y-index, nXelem, nYelem, [band-index])
      template<typename T> void getBlock(T*,size_t,size_t,size_t,size_t,size_t);
      template<typename T> void getBlock(T*,size_t,size_t,size_t,size_t);
      template<typename T> void getBlock(std::vector<T>&,size_t,size_t,size_t,size_t,size_t);
      template<typename T> void getBlock(std::vector<T>&,size_t,size_t,size_t,size_t);

      // Single block read, 2D STL containers, optional band index
      // (buffer, x-index, y-index, [band-index])
      template<typename T> void getBlock(std::vector<std::vector<T>>&,size_t,size_t,size_t);
      template<typename T> void getBlock(std::vector<std::vector<T>>&,size_t,size_t);

      // Single block write, linear containers w/ x/y sizes, optional band index
      // (buffer, x-index, y-index, nXelem, nYelem, [band-index])
      template<typename T> void setBlock(T*,size_t,size_t,size_t,size_t,size_t);
      template<typename T> void setBlock(T*,size_t,size_t,size_t,size_t);
      template<typename T> void setBlock(std::vector<T>&,size_t,size_t,size_t,size_t,size_t);
      template<typename T> void setBlock(std::vector<T>&,size_t,size_t,size_t,size_t);

      // Single block write, 2D STL containers, optional band index
      // (buffer, x-index, y-index, [band-index])
      template<typename T> void setBlock(std::vector<std::vector<T>>&,size_t,size_t,size_t);
      template<typename T> void setBlock(std::vector<std::vector<T>>&,size_t,size_t);
      
    private:
      GDALDataset * _dataset; 
      GDALAccess    _access; // from gdal.h: typedef enum { GA_ReadOnly=0, GA_Update=1 } GDALAccess; 
      static const std::unordered_map<std::type_index, GDALDataType> _gdts;
    };
    
  }
}

#define ISCE_CORE_RASTER_ICC
#include "Raster.icc"
#undef ISCE_CORE_RASTER_ICC

#endif
