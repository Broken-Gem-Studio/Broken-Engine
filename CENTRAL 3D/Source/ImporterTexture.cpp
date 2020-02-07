#include "ImporterTexture.h"
#include "Application.h"
#include "ModuleResources.h"
#include "ModuleTextures.h"

#include "ImporterMeta.h"
#include "ResourceMeta.h"
#include "ResourceTexture.h"

ImporterTexture::ImporterTexture() : Importer(Importer::ImporterType::Texture)
{
}

ImporterTexture::~ImporterTexture()
{
}

Resource* ImporterTexture::Import(ImportData& IData) const
{
	ResourceTexture* texture = (ResourceTexture*)App->resources->CreateResource(Resource::ResourceType::TEXTURE, IData.path);;

	// --- Create Meta ---
	ImporterMeta* IMeta = App->resources->GetImporter<ImporterMeta>();

	ResourceMeta* meta = (ResourceMeta*)App->resources->CreateResourceGivenUID(Resource::ResourceType::META, texture->GetOriginalFile(), texture->GetUID());

	if (meta)
		IMeta->Save(meta);


	Save(texture);

	return texture;
}

Resource* ImporterTexture::Load(const char* path) const
{
	ResourceTexture* texture = nullptr;

	ImporterMeta* IMeta = App->resources->GetImporter<ImporterMeta>();
	ResourceMeta* meta = (ResourceMeta*)IMeta->Load(path);

	texture = App->resources->textures.find(meta->GetUID()) != App->resources->textures.end() ? App->resources->textures.find(meta->GetUID())->second : (ResourceTexture*)App->resources->CreateResourceGivenUID(Resource::ResourceType::TEXTURE, path, meta->GetUID());

	return texture;
}

void ImporterTexture::Save(ResourceTexture* texture) const
{
	// Nothing to save ?
}