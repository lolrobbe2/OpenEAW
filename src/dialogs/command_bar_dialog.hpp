#pragma once

#include "game_mode_stack.hpp"

#include <openglyph/ui/context.hpp>

#include <vector>
namespace openeaw {

class CommandBar
{
public:
    explicit CommandBar(openglyph::AssetLoader& asset_loader, openglyph::AssetCache& asset_cache);
    const std::shared_ptr<khepri::scene::SceneObject> p_scene_object = std::make_shared<khepri::scene::SceneObject>()
private:
    class CommandBarComponent
    {
    private:
        enum class Type
        {
            shell,
            button,
            text_button,
            icon,
            bar,
        };
        [[nodiscard]] constexpr std::optional<Type>
        type_from_string(std::string_view value) noexcept
        {
            if (value == "Shell")
                return Type::shell;

            if (value == "Button")
                return Type::button;

            if (value == "TextButton")
                return Type::text_button;

            if (value == "Icon")
                return Type::icon;

            if (value == "Bar")
                return Type::bar;

            return std::nullopt;
        }

    public:
        explicit CommandBarComponent(const openglyph::XmlParser::Node& node,
                                     openglyph::AssetCache&            asset_cache);
        std::string_view m_name;
        std::string_view m_model_name;
        std::string_view m_mega_texture_name;

        Type m_type;

        bool m_disabled       = true;
        bool m_model_offset_x = false;
        bool m_model_offset_y = false;
        bool m_manual_offset  = false;
        bool m_hidden         = false;
        
        const openglyph::renderer::RenderModel* p_model = nullptr;
    };

    [[nodiscard]] std::vector<std::string_view>
    load_component_files(openglyph::AssetLoader& asset_loader);

    void load_component_file(openglyph::AssetLoader& asset_loader,
                             openglyph::AssetCache& asset_cache, std::string_view filename);

    std::vector<CommandBarComponent> m_components;
};

} // namespace openeaw
