#include "command_bar_dialog.hpp"

#include <openglyph/parser/parsers.hpp>

namespace openeaw {
constexpr khepri::log::Logger LOG("font");

std::vector<std::string_view> CommandBar::load_component_files(openglyph::AssetLoader& asset_loader)
{
    std::vector<std::string_view> component_files;
    if (auto stream = asset_loader.open_config("CommandBarComponentFiles")) {
        openglyph::XmlParser parser(*stream);
        if (const auto& root = parser.root()) {
            for (const auto& node : root->nodes()) {
                try {
                    if (node.name() != "File") {
                        LOG.error("Expected File node");
                        continue;
                    }
                    component_files.push_back(node.value());
                } catch (openglyph::ParseError& e) {
                    LOG.error("Error reading command bar component files: {}", e.what());
                }
            }
        }
    }
    return component_files;
}

void CommandBar::load_component_file(openglyph::AssetLoader& asset_loader,
                                     openglyph::AssetCache& asset_cache, std::string_view filename)
{
    if (auto stream = asset_loader.open_config(filename)) {
        openglyph::XmlParser parser(*stream);
        if (const auto& root = parser.root()) {
            for (const auto& node : root->nodes()) {
                try {
                    if (node.name() != "CommandBarComponent") {
                        LOG.error("Expected CommandBarComponent node");
                        continue;
                    }
                    m_components.emplace_back(CommandBarComponent(node, asset_cache));
                } catch (openglyph::ParseError& e) {
                    LOG.error("Error reading command bar component file: {}", e.what());
                }
            }
        }
    }
}

CommandBar::CommandBar(openglyph::AssetLoader& asset_loader, openglyph::AssetCache& asset_cache)
{
    for (const auto& component_file : load_component_files(asset_loader)) {
        load_component_file(asset_loader, asset_cache, component_file);
    }
}

CommandBar::CommandBarComponent::CommandBarComponent(const openglyph::XmlParser::Node& node,
                                                     openglyph::AssetCache&            asset_cache)
{
    std::optional<std::string_view> name_attr = node.attribute("Name");
    if (!name_attr.has_value()) {
        throw openglyph::ParseError("CommandBarComponent needs to have a name attribute");
    }

    m_name = name_attr.value();
    for (const auto& child : node.nodes()) {
        std::string_view name = child.name();
        std::string_view value = child.value();
        if (name == "Model_Name") {
            m_model_name = value;
            p_model      = asset_cache.get_render_model(m_model_name);
            continue;
        } else if (name == "Mega_Texture_Name") {
            m_mega_texture_name = value;
            continue;
        } else if (name == "Type") {
            m_type = type_from_string(value).value();
            continue;
        }
    }
}

} // namespace openeaw