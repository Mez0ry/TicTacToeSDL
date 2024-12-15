#include "Menu.hpp"
#include "SceneManager.hpp"

#include "Playing.hpp"
#include "Settings.hpp"
#include "Exit.hpp"

#include <type_traits>
#include "Easing.hpp"

Menu::Menu(const Core::Ref<Renderer> renderer, const Core::Ref<Window> window,SceneManager& scene_manager) : m_Renderer(renderer), m_SceneManager(scene_manager){
    auto [win_w,win_h] = window->GetWindowSize();

    m_TitlePanelTexture.LoadTexture(renderer,"resources/UI/TitlePanel01.png");
    m_TitlePanelTexture.SetSize(ObjectSize(501,36));
    
    m_BackgroundTexture.LoadTexture(renderer,"resources/UI/MainPanel01.png");
    
    m_BackgroundTexture.SetSize(ObjectSize(win_w,win_h));

    m_TitleText.LoadFont("resources/fonts/Wombyland/OpenType-TT/WombyLand.ttf",36);
    m_TitleText.LoadText(renderer,"Tic-Tac-Toe",{172,7,62,230});

    m_TitleText->SetSize(ObjectSize(240,36));

    Texture button_texture(renderer,"resources/UI/Button16.png");
    auto buttons_font = Text::StaticLoadFont("resources/fonts/Aileron/Aileron-SemiBold.otf",100);
    
    Core::Ref<Button> play_button = Core::CreateRef<Button>(button_texture,"resources/fonts/Aileron/Aileron-SemiBold.otf","Play",m_ButtonColor);
    Core::Ref<Button> settings_button = Core::CreateRef<Button>(button_texture,"resources/fonts/Aileron/Aileron-SemiBold.otf","Settings",m_ButtonColor);
    Core::Ref<Button> exit_button = Core::CreateRef<Button>(button_texture,"resources/fonts/Aileron/Aileron-SemiBold.otf","Exit",m_ButtonColor);

    play_button->OnClick([&](){
        m_SceneManager.TransitionTo<Menu,Playing>();
    });

    exit_button->OnClick([&](){
        m_SceneManager.TransitionTo<Menu,Exit>();
    });

    settings_button->OnClick([&](){
        m_SceneManager.TransitionTo<Menu,Settings>();
    });

    m_Buttons.push_back(play_button);
    m_Buttons.push_back(settings_button);
    m_Buttons.push_back(exit_button);
    
    OnResize(window);

    m_TargetPlayButtonPos = play_button->GetPosition();
    m_TargetExitButtonPos = exit_button->GetPosition();
    m_TargetSettingsButtonPos = settings_button->GetPosition(); 

    m_DefaultPlayButtonPosX = 0 - play_button->GetSize().GetWidth();
    m_DefaultSettingsButtonPosX = win_w + settings_button->GetSize().GetWidth();
    m_DefaultExitButtonPosX = 0 - exit_button->GetSize().GetWidth();

    m_ButtonKeyFrame.Setup(3,[&](float t){
        auto color = m_ButtonColor;
        color.a = (Stellar::Lerp(0,255,Stellar::Easing::EaseOutQuad(t)));

        int dx_play_button =  (Stellar::Lerp(m_DefaultPlayButtonPosX,m_TargetPlayButtonPos.x,Stellar::Easing::EaseInOutCubic(t)));
        m_Buttons[0]->SetPosition({dx_play_button,m_TargetPlayButtonPos.y});
        m_Buttons[0]->ChangeTextColor(color);

        int dx_settings_button =  (Stellar::Lerp(m_DefaultSettingsButtonPosX,m_TargetSettingsButtonPos.x,Stellar::Easing::EaseInOutCubic(t)));
        m_Buttons[1]->SetPosition({dx_settings_button,m_TargetSettingsButtonPos.y});
        m_Buttons[1]->ChangeTextColor(color);

        int dx_exit_button =  (Stellar::Lerp(m_DefaultExitButtonPosX,m_TargetExitButtonPos.x,Stellar::Easing::EaseInOutCubic(t)));
        m_Buttons[2]->SetPosition({dx_exit_button,m_TargetExitButtonPos.y});
        m_Buttons[2]->ChangeTextColor(color);
    });

    m_TmpTitleTextPos = m_TitleText->GetPosition();
    m_TmpPanelTexturePos = m_TitlePanelTexture.GetPosition();

    m_TitleKFOut.Setup(2,[&](float t){
        int dy_text =  (Stellar::Lerp(m_TmpTitleTextPos.y + 15,m_TmpTitleTextPos.y,Stellar::Easing::EaseOutBounce(t)));
        int dy_panel = (Stellar::Lerp(m_TmpPanelTexturePos.y + 15,m_TmpPanelTexturePos.y,Stellar::Easing::EaseOutBounce(t)));

        Vec2i new_text_pos = {m_TmpTitleTextPos.x,dy_text};
        Vec2i new_panel_pos = {m_TmpPanelTexturePos.x,dy_panel};

        m_TitleText->SetPosition(new_text_pos);
        m_TitlePanelTexture.SetPosition(new_panel_pos);
    });

    m_TitleKFIn.Setup(4,[&](float t){

        int dy_text =  (Stellar::Lerp(m_TmpTitleTextPos.y,m_TmpTitleTextPos.y + 15,Stellar::Easing::EaseInBounce(t)));
        int dy_panel = (Stellar::Lerp(m_TmpPanelTexturePos.y ,m_TmpPanelTexturePos.y + 15,Stellar::Easing::EaseInBounce(t)));

        Vec2i new_text_pos = {m_TmpTitleTextPos.x,dy_text};
        Vec2i new_panel_pos = {m_TmpPanelTexturePos.x,dy_panel};

        m_TitleText->SetPosition(new_text_pos);
        m_TitlePanelTexture.SetPosition(new_panel_pos);
    });
}

Menu::~Menu(){

}

void Menu::OnResize(const Core::Ref<Window> window) {
    int win_w{},win_h {};

    std::tie(win_w,win_h) = window->GetWindowSize();

    ObjectSize menu_option_size_dst(125,65);

    int y_offset = 0;
    
    for(auto& button : m_Buttons){

        Vec2 pos;
        pos.x = (win_w / 2) - (menu_option_size_dst.GetWidth() / 2);
        pos.y = ((win_h / 2) - (menu_option_size_dst.GetHeight() * 2)) + y_offset;
        
        button->SetRect(pos,menu_option_size_dst);
        
        y_offset += (menu_option_size_dst.GetHeight() * 2);
    }

    auto title_size = m_TitleText->GetSize();

    m_TitleText->SetPosition({win_w / 2 - (title_size.GetWidth() / 2), static_cast<int>(win_h * 0.1f)});

    ObjectSize title_panel_size(title_size.GetWidth() * 1.3f,title_size.GetHeight() * 1.5f);
    m_TitlePanelTexture.SetSize(title_panel_size);

    ObjectSize diff = m_TitlePanelTexture.GetSize() - title_size;
    Vec2i pos;
    
    pos.x = (m_TitleText->GetPosition().x - (diff.GetWidth() / 2));
    pos.y = (m_TitleText->GetPosition().y - (diff.GetHeight() / 2));

    m_TitlePanelTexture.SetPosition(pos);
    m_BackgroundTexture.SetSize(ObjectSize(win_w + 100,win_h + 100));
    Vec2i centered_pos = {-50,-50};
    m_BackgroundTexture.SetPosition(centered_pos);

}

void Menu::OnCreate(){
    m_Renderer->SetRenderDrawColor({0,255,212,250});
    m_TitleKFIn.Restart();
    m_TitleKFOut.Restart();
}

void Menu::OnDestroy()
{

}

void Menu::HandleInput(const Core::Ref<EventHandler> event_handler){
  for(auto& button : m_Buttons){
    if(button->IsClicked()){
        button->Execute();
    }
    
    if(button->IsHovered()){
        button->ChangeTextColor(color_red_t);
    }else{
        button->ChangeTextColor(m_ButtonColor);
    }
  }
}

void Menu::Update(float dt){
    if(m_TitleKFIn.Update(dt)){
        m_TitleKFOut.Update(dt);
    }

    m_ButtonKeyFrame.Update(dt);
}

void Menu::Render(const Core::Ref<Renderer> renderer){
    renderer->SetRenderDrawColor({0,255,212,250});

    renderer->Render(m_BackgroundTexture);
    
    for(auto& button : m_Buttons){
        if(button){
            button->Render(renderer);
        }
    }
    
    renderer->Render(m_TitlePanelTexture);
    renderer->Render(m_TitleText);
}