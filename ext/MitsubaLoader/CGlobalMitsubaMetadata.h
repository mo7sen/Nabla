// Copyright (C) 2018-2020 - DevSH Graphics Programming Sp. z O.O.
// This file is part of the "Nabla Engine".
// For conditions of distribution and use, see copyright notice in nabla.h

#ifndef __C_GLOBAL_MITSUBA_METADATA_H_INCLUDED__
#define __C_GLOBAL_MITSUBA_METADATA_H_INCLUDED__

#include "../../ext/MitsubaLoader/CElementIntegrator.h"
#include "../../ext/MitsubaLoader/CElementSensor.h"
#include "../../ext/MitsubaLoader/CElementShape.h"
#include "../../ext/MitsubaLoader/SContext.h"
#include <irr/asset/ICPUVirtualTexture.h>

namespace irr
{
namespace ext
{
namespace MitsubaLoader
{

class CGlobalMitsubaMetadata : public core::IReferenceCounted
{
	public:
		CGlobalMitsubaMetadata() : integrator("")
		{
			integrator.type = CElementIntegrator::Type::INVALID;
		}

		CElementIntegrator integrator;
		core::vector<CElementSensor> sensors;
		core::vector<CElementEmitter> emitters;
		core::smart_refctd_ptr<asset::ICPUVirtualTexture> VT;
};



//! TODO: move these to separate files


class IMitsubaMetadata : public asset::IAssetMetadata
{
	public:
		IMitsubaMetadata(core::smart_refctd_ptr<CGlobalMitsubaMetadata>&& _gmeta, std::string&& _id="") : globalMetadata(_gmeta), id(_id) {}

		_NBL_STATIC_INLINE_CONSTEXPR const char* LoaderName = "Mistuba XML";
		const char* getLoaderName() const override {return LoaderName;}


		const core::smart_refctd_ptr<CGlobalMitsubaMetadata> globalMetadata;
		const std::string id;
};

// nested <shapes>
class IMeshMetadata : public IMitsubaMetadata
{
	public:
		IMeshMetadata(core::smart_refctd_ptr<CGlobalMitsubaMetadata>&& _gmeta, std::string&& _id, CElementShape* shape) :
			IMitsubaMetadata(std::move(_gmeta),std::move(_id)), type(shape->type)
		{}

		inline auto getShapeType() const {return type;}

		struct Instance
		{
			core::matrix3x4SIMD tform;
			SContext::bsdf_type bsdf;
			std::string bsdf_id;
			CElementEmitter emitter; // type is invalid if not used
		};

		inline const auto& getInstances() const { return instances; }

	protected:
		CElementShape::Type type;
		core::vector<Instance> instances;

		friend class CMitsubaLoader;
};

// <shape>
class IMeshBufferMetadata : public IMitsubaMetadata
{
};

}
}
}

#endif