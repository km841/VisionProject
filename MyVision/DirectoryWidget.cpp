#include "DirectoryWidget.h"
#include "Texture.h"
#include "RenderManager.h"

DirectoryWidget::DirectoryWidget(const Vec2& _Size)
    : Widget(_Size)
    , MSelectedImageIndex(-1)
{
}

DirectoryWidget::~DirectoryWidget()
{
}

void DirectoryWidget::Initialize()
{
    PIconTexture = std::make_shared<Texture>(OBJECT_TYPE::TEXTURE);
    PIconTexture->Load(L"..\\Resources\\Icon\\Image.png");

}

void DirectoryWidget::Update()
{
    VDirectoryImagePath.clear();
    for (auto DirIter : fs::directory_iterator(MDefaultPath))
    {
        VDirectoryImagePath.push_back(DirIter.path().filename().string());
    }

    const int ImageCount = static_cast<int>(VDirectoryImagePath.size());
    if (ImGui::Begin("Directory", nullptr))
    {
        for (int i = 0; i < ImageCount; ++i) {
            ImGui::PushID(i);
            ImGui::ImageButton(PIconTexture->GetSRV().Get(), Vec2(100.f, 100.f));
            
            if (ImGui::IsItemClicked())
            {
                MSelectedImageIndex = i;
                RENDER->SetTexture(s2ws(MDefaultPath + VDirectoryImagePath[i]));
            }
            ImGui::PopID();
            
            ImGui::SameLine(0.0f, 30.f);

            ImGui::SetWindowFontScale(2.f);

            Vec2 TextPosition = ImGui::GetCursorScreenPos();
            Vec2 PrevPosition = TextPosition;

            TextPosition.y += 70.f;

           
            ImGui::SetCursorScreenPos(TextPosition);

            ImGui::Text(VDirectoryImagePath[i].c_str());
            ImGui::SetCursorScreenPos(PrevPosition);

            ImGui::SameLine(0.0f, 80.f);

        }
    }
    ImGui::End();
}
