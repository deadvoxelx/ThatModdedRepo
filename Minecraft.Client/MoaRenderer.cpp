#include "stdafx.h"
#include "MoaRenderer.h"
#include "MoaModel.h"
#include "..\Minecraft.World\Mth.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"

ResourceLocation MoaRenderer::COCKATRICE_LOCATION = ResourceLocation(TN_MOB_COCKATRICE);
ResourceLocation MoaRenderer::MOA_LOCATION = ResourceLocation(TN_MOB_MOABLUE);
ResourceLocation MoaRenderer::MOA_WHITE_LOCATION = ResourceLocation(TN_MOB_MOAWHITE);
ResourceLocation MoaRenderer::MOA_BLACK_LOCATION = ResourceLocation(TN_MOB_MOABLACK);

MoaRenderer::MoaRenderer() : MobRenderer(new MoaModel(), 1.0f)
{
	//this->setArmor(new MoaModel());
}

void MoaRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *MoaRenderer::getTextureLocation(shared_ptr<Entity> entity)
{
	shared_ptr<Cockatrice> cockatrice = dynamic_pointer_cast<Cockatrice>(entity);
	shared_ptr<Moa> moa = dynamic_pointer_cast<Moa>(entity);

	if (entity->instanceof(eTYPE_COCKATRICE))
	{
        return &COCKATRICE_LOCATION;
    }

    if (moa->getMoaType() == Moa::TYPE_DEFAULT)
	{
        return &MOA_LOCATION;
    }
    if (moa->getMoaType() == Moa::TYPE_WHITE)
	{
        return &MOA_WHITE_LOCATION;
    }
	if (moa->getMoaType() == Moa::TYPE_BLACK)
	{
        return &MOA_BLACK_LOCATION;
    }
}

void MoaRenderer::scale(shared_ptr<LivingEntity> mob, float a)
{
	glScalef(2.0f, 2.0f, 2.0f);
}

float MoaRenderer::getBob(shared_ptr<LivingEntity> _mob, float a)
{
	shared_ptr<Cockatrice> mob = dynamic_pointer_cast<Cockatrice>(_mob);
	shared_ptr<Moa> moa = dynamic_pointer_cast<Moa>(_mob);

	if (mob != nullptr)
	{
		float flap = mob->oFlap+(mob->flap-mob->oFlap)*a;
		float flapSpeed = mob->oFlapSpeed+(mob->flapSpeed-mob->oFlapSpeed)*a;
		return (Mth::sin(flap)+1)*flapSpeed;
	}

	if (moa != nullptr)
	{
		float flapm = moa->oFlapM+(moa->flapM-moa->oFlapM)*a;
		float flapSpeedm = moa->oFlapSpeedM+(moa->flapSpeedM-moa->oFlapSpeedM)*a;
		return (Mth::sin(flapm)+1)*flapSpeedm;
	}

	return 0.0f;
}

/*int EvupulRenderer::prepareArmor(shared_ptr<LivingEntity> _mob, int layer, float a)
{
	shared_ptr<EvupulDark> evupulDark = dynamic_pointer_cast<EvupulDark>(_mob);
	shared_ptr<Evupul> evupul = dynamic_pointer_cast<Evupul>(_mob);

	if (layer!=0) return -1;
	MemSect(31);
	if (shared_ptr<LivingEntity>(evupulDark))
	{
        bindTexture(&EVUPULDARK_OVERLAY_LOCATION);
    }
	else
	{
		if (evupul->getEvupulType() == Evupul::TYPE_DEFAULT)
		{
			bindTexture(&EVUPUL_OVERLAY_LOCATION);
		}
		else if (evupul->getEvupulType() == Evupul::TYPE_GOLD)
		{
			bindTexture(&EVUPULGOLD_OVERLAY_LOCATION);
		}
    }
	MemSect(0);
	float br = 1.0f;
	glEnable(GL_BLEND);
#ifdef _XBOX
	glDisable(GL_ALPHA_TEST);
#endif
	glBlendFunc(GL_ONE, GL_ONE);
	//if (evupulDark->isInvisible())	glDepthMask(false);
	//else						glDepthMask(true);

	if (SharedConstants::TEXTURE_LIGHTING)
	{
		int col = 0x00f0;			
		int u = col % 65536;
		int v = col / 65536;

		glMultiTexCoord2f(GL_TEXTURE1, u / 1.0f, v / 1.0f);
		glColor4f(1, 1, 1, 1);
	}
	glColor4f(1, 1, 1, br);
	return 1;
}*/
