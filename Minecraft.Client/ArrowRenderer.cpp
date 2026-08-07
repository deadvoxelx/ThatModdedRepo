#include "stdafx.h"
#include "ArrowRenderer.h"
#include "..\Minecraft.World\Mth.h"
#include "..\Minecraft.World\net.minecraft.world.entity.projectile.h"

ResourceLocation ArrowRenderer::ARROW_LOCATION = ResourceLocation(TN_ITEM_ARROWS);
ResourceLocation ArrowRenderer::DART_LOCATION = ResourceLocation(TN_MOB_DART);
ResourceLocation ArrowRenderer::DART_ENCHANTED_LOCATION = ResourceLocation(TN_MOB_DARTENCHANTED);

void ArrowRenderer::render(shared_ptr<Entity> _entity, double x, double y, double z, float rot, float a)
{
	shared_ptr<Arrow> arrow = dynamic_pointer_cast<Arrow>(_entity);
    shared_ptr<Dart> dart = dynamic_pointer_cast<Dart>(_entity);
    shared_ptr<DartEnchanted> dartE = dynamic_pointer_cast<DartEnchanted>(_entity);

    if (shared_ptr<Entity>(arrow))
	{
        bindTexture(_entity);
    }
    if (shared_ptr<Entity>(dart))
	{
        bindTexture(_entity);
    }
    if (shared_ptr<Entity>(dartE))
	{
        bindTexture(_entity);
    }

    glPushMatrix();

	float yRot = _entity->yRot;
	float xRot = _entity->xRot;
	float yRotO = _entity->yRotO;
	float xRotO = _entity->xRotO;
	if( ( yRot - yRotO ) > 180.0f ) yRot -= 360.0f;
	else if( ( yRot - yRotO ) < -180.0f ) yRot += 360.0f;
	if( ( xRot - xRotO ) > 180.0f ) xRot -= 360.0f;
	else if( ( xRot - xRotO ) < -180.0f ) xRot += 360.0f;

    glTranslatef(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
    glRotatef(yRotO + (yRot - yRotO) * a - 90, 0, 1, 0);
    glRotatef(xRotO + (xRot - xRotO) * a, 0, 0, 1);

    Tesselator *t = Tesselator::getInstance();

    int type = 0;
    float u0 = 0 / 32.0f;
    float u1 = 16 / 32.0f;
    float v0 = (0 + type * 10) / 32.0f;
    float v1 = (5 + type * 10) / 32.0f;
    float u02 = 0 / 32.0f;
    float u12 = 5 / 32.0f;
    float v02 = (5 + type * 10) / 32.0f;
    float v12 = (10 + type * 10) / 32.0f;
    float ss = 0.9f / 16.0f;

    glEnable(GL_RESCALE_NORMAL);

    float shake = 0.0f;
	if (arrow != nullptr) shake = arrow->shakeTime - a;
	else if (dart != nullptr) shake = static_cast<float>(dart->shakeTime) - a;
	else if (dartE != nullptr) shake = static_cast<float>(dartE->shakeTime) - a;

    if (shake>0)
    {
        float pow = -Mth::sin(shake*3)*shake;
        glRotatef(pow, 0, 0, 1);
    }

    glRotatef(45, 1, 0, 0);
    glScalef(ss, ss, ss);
    glTranslatef(-4, 0, 0);

    t->begin();
	t->normal(1,0,0);
    t->vertexUV(static_cast<float>(-7), static_cast<float>(-2), static_cast<float>(-2), (float)( u02), (float)( v02));
    t->vertexUV(static_cast<float>(-7), static_cast<float>(-2), static_cast<float>(+2), (float)( u12), (float)( v02));
    t->vertexUV(static_cast<float>(-7), static_cast<float>(+2), static_cast<float>(+2), (float)( u12), (float)( v12));
    t->vertexUV(static_cast<float>(-7), static_cast<float>(+2), static_cast<float>(-2), (float)( u02), (float)( v12));
    t->end();

    t->begin();
	t->normal(-1,0,0);
    t->vertexUV(static_cast<float>(-7), static_cast<float>(+2), static_cast<float>(-2), (float)( u02), (float)( v02));
    t->vertexUV(static_cast<float>(-7), static_cast<float>(+2), static_cast<float>(+2), (float)( u12), (float)( v02));
    t->vertexUV(static_cast<float>(-7), static_cast<float>(-2), static_cast<float>(+2), (float)( u12), (float)( v12));
    t->vertexUV(static_cast<float>(-7), static_cast<float>(-2), static_cast<float>(-2), (float)( u02), (float)( v12));
    t->end();

    for (int i = 0; i < 4; i++)
	{

        glRotatef(90, 1, 0, 0);

        t->begin();
		t->normal(0,0,1);
        t->vertexUV(static_cast<float>(-8), static_cast<float>(-2), static_cast<float>(0), (float)( u0), (float)( v0));
        t->vertexUV(static_cast<float>(+8), static_cast<float>(-2), static_cast<float>(0), (float)( u1), (float)( v0));
        t->vertexUV(static_cast<float>(+8), static_cast<float>(+2), static_cast<float>(0), (float)( u1), (float)( v1));
        t->vertexUV(static_cast<float>(-8), static_cast<float>(+2), static_cast<float>(0), (float)( u0), (float)( v1));
        t->end();
    }

    glDisable(GL_RESCALE_NORMAL);
    glPopMatrix();
}

ResourceLocation *ArrowRenderer::getTextureLocation(shared_ptr<Entity> entity)
{
    shared_ptr<Arrow> arrow = dynamic_pointer_cast<Arrow>(entity);
    shared_ptr<Dart> dart = dynamic_pointer_cast<Dart>(entity);
    shared_ptr<DartEnchanted> dartE = dynamic_pointer_cast<DartEnchanted>(entity);

    if (entity->instanceof(eTYPE_ARROW))
	{
        return &ARROW_LOCATION;
    }
    if (entity->instanceof(eTYPE_DART))
	{
        return &DART_LOCATION;
    }
    if (entity->instanceof(eTYPE_DARTENCHANTED))
	{
        return &DART_ENCHANTED_LOCATION;
    }
}
