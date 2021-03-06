#pragma once
#include <Core/CoreHeader.h>
#include <Render/RenderConst.h>
#include <Render/Material.h>
#include <Render/Mesh.h>
#include <Render/RenderOption.h>
#include <RHI/RHIDevice.h>

#include "RenderStage.h"

namespace z {

class RenderItem : public RefCounter{
public:
	ERenderSet RenderSet;

	math::Matrix4F WorldMatrix;
	RefCountPtr<RenderMesh> Mesh;
	RefCountPtr<MaterialInstance> Material;

public:
	RenderItem() :
		mMeshIndexGroup(0),
		mMeshVertexGroup(0),
		RenderSet(RENDER_SET_OPAQUE) {
	}

	void SetMeshIndexGroup(int idx) {
		mMeshIndexGroup = idx;
	}
	
	void SetMeshVertexGroup(int idx) {
		mMeshVertexGroup = idx;
	}

	void RetriveItemParams() {
		// parameter
		Material->SetParameter("World", (const float*)&WorldMatrix, 16);

		// render option
		int option = GRenderOptions.HDR ? 1 : 0;
		Material->SetParameter("EnableHDR", &option, 1);

	}


	void Draw() {
		auto [vb, ib] = Mesh->GetRHIResource();
		int num = Mesh->GetIndexCount(mMeshIndexGroup);
		int baseIndex = Mesh->GetIndexOffset(mMeshIndexGroup);
		int baseVertex = Mesh->GetVertexOffset(mMeshVertexGroup);

		RetriveItemParams();
		RenderStage::Apply();
		GDevice->DrawIndexed(Material->GetShaderInstance(), vb, ib, Material->mRState, num, baseIndex, baseVertex);
	}

	void CustomDraw(int indexNum, int indexOffset, int vertexOffset) {
		auto [vb, ib] = Mesh->GetRHIResource();
		int baseIndex = Mesh->GetIndexOffset(mMeshIndexGroup) + indexOffset;
		int baseVertex = Mesh->GetVertexOffset(mMeshVertexGroup) + vertexOffset;

		RetriveItemParams();
		RenderStage::Apply();
		GDevice->DrawIndexed(Material->GetShaderInstance(), vb, ib, Material->mRState, indexNum, baseIndex, baseVertex);
	}

private:
	int mMeshIndexGroup;
	int mMeshVertexGroup;
};

}