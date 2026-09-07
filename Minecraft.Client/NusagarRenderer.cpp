#include "stdafx.h"
#include "NusagarRenderer.h"
#include "NusagarModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"
#include "..\Minecraft.World\Nusagar.h"
#include "BossMobGuiInfo.h"

ResourceLocation NusagarRenderer::NUSAGAR_LOCATION = ResourceLocation(TN_MOB_NUSAGAR);

NusagarRenderer::NusagarRenderer() : MobRenderer(new NusagarModel(), 0.7f)
{
	modelVersion = static_cast<NusagarModel *>(model)->modelVersion();
}

void NusagarRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	shared_ptr<Nusagar> nusagar = dynamic_pointer_cast<Nusagar>(_mob);
	if (nusagar != nullptr)
	{
		BossMobGuiInfo::setBossHealth(nusagar, true);
	}

	int modelVersion = (dynamic_cast<NusagarModel*>(model))->modelVersion();
	if (modelVersion != this->modelVersion) {
		this->modelVersion = modelVersion;
		model = new NusagarModel();
	}
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *NusagarRenderer::getTextureLocation(shared_ptr<Entity> entity)
{
	return &NUSAGAR_LOCATION;
}

void NusagarRenderer::setupPosition(shared_ptr<LivingEntity> mob, double x, double y, double z)
{
	MobRenderer::setupPosition(mob, x, y, z);
}