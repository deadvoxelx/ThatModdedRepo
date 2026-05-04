#include "stdafx.h"
#include "AphalafBossRenderer.h"
#include "AphalafBossModel.h"
#include "MobRenderer.h"
#include "../Minecraft.World/AphalafBoss.h"
#include "../Minecraft.Client/BossMobGuiInfo.h"

ResourceLocation AphalafBossRenderer::APHALAF_BOSS_LOCATION = ResourceLocation(TN_MOB_APHALAF_BOSS);

AphalafBossRenderer::AphalafBossRenderer() : MobRenderer(new AphalafBossModel(), 1.0f)
{
	modelVersion = dynamic_cast<AphalafBossModel*>(model)->modelVersion();
}

void AphalafBossRenderer::scale(shared_ptr<LivingEntity> _mob, float a)
{
	glScalef(3.5, 3.5, 3.5);
}

void AphalafBossRenderer::render(shared_ptr<Entity> entity, double x, double y, double z, float rot, float a)
{
	shared_ptr<AphalafBoss> mob = dynamic_pointer_cast<AphalafBoss>(entity);

	BossMobGuiInfo::setBossHealth(mob, true);

	int modelVersion = dynamic_cast<AphalafBossModel*>(model)->modelVersion();
	if (modelVersion != this->modelVersion)
	{
		this->modelVersion = modelVersion;
		model = new AphalafBossModel();
	}
	MobRenderer::render(entity, x, y, z, rot, a);
}

ResourceLocation *AphalafBossRenderer::getTextureLocation(shared_ptr<Entity> entity)
{
	shared_ptr<AphalafBoss> mob = dynamic_pointer_cast<AphalafBoss>(entity);

    return &APHALAF_BOSS_LOCATION;
}