// Copyright (C) Gaijin Games KFT.  All rights reserved.
#pragma once

#include "av_assetTypeFilterControl.h"
#include "av_favoritesTree.h"
#include <propPanel/focusHelper.h>
#include <propPanel/imguiHelper.h>
#include <propPanel/propPanel.h>
#include <imgui/imgui.h>

class FavoritesTab
{
public:
  explicit FavoritesTab(SelectAssetDlg &select_asset_dlg, const DagorAssetMgr &asset_mgr) :
    favoritesTree(select_asset_dlg), assetMgr(asset_mgr)
  {
    closeIcon = (ImTextureID)((unsigned)PropPanel::load_icon("close_editor"));
    searchIcon = (ImTextureID)((unsigned)PropPanel::load_icon("search"));
    settingsIcon = (ImTextureID)((unsigned)PropPanel::load_icon("filter_default"));
    settingsOpenIcon = (ImTextureID)((unsigned)PropPanel::load_icon("filter_active"));
  }

  void fillTree() { favoritesTree.fillTree(); }

  DagorAsset *getSelectedAsset() { return favoritesTree.getSelectedAsset(); }

  void onAllowedTypesChanged(dag::ConstSpan<int> allowed_type_indexes)
  {
    allowedTypes.clear();
    shownTypes.clear();
    allowedTypes.resize(assetMgr.getAssetTypesCount());
    shownTypes.resize(assetMgr.getAssetTypesCount());

    for (int i = 0; i < assetMgr.getAssetTypesCount(); ++i)
    {
      const bool allowed = eastl::find(allowed_type_indexes.begin(), allowed_type_indexes.end(), i) != allowed_type_indexes.end();
      allowedTypes[i] = shownTypes[i] = allowed;
    }

    onShownTypeFilterChanged();
  }

  void updateImgui()
  {
    const ImVec2 fontSizedIconSize = PropPanel::ImguiHelper::getFontSizedIconSize();
    const ImVec2 settingsButtonSize = PropPanel::ImguiHelper::getImageButtonWithDownArrowSize(fontSizedIconSize);

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (ImGui::GetStyle().ItemSpacing.x + settingsButtonSize.x));
    if (PropPanel::ImguiHelper::searchInput(&searchInputFocusId, "##searchInput", "Search", favoritesTree.getTextToSearch(),
          searchIcon, closeIcon))
      favoritesTree.fillTree();
    ImGui::SameLine();

    const char *popupId = "settingsPopup";
    const ImTextureID settingsButtonIcon = settingsPanelOpen ? settingsOpenIcon : settingsIcon;
    const bool settingsButtonPressed =
      PropPanel::ImguiHelper::imageButtonWithArrow("settingsButton", settingsButtonIcon, fontSizedIconSize, settingsPanelOpen);
    PropPanel::set_previous_imgui_control_tooltip((const void *)ImGui::GetItemID(), "Settings");

    if (settingsButtonPressed)
    {
      ImGui::OpenPopup(popupId);
      settingsPanelOpen = true;
    }

    if (settingsPanelOpen)
      showSettingsPanel(popupId);

    if (favoritesTree.getShowHierarchy())
    {
      const char *expandAllTitle = "Expand all";
      const char *collapseAllTitle = "Collapse all";
      const ImVec2 expandAllSize = PropPanel::ImguiHelper::getButtonSize(expandAllTitle);
      const ImVec2 collapseAllSize = PropPanel::ImguiHelper::getButtonSize(collapseAllTitle);
      const ImVec2 expandButtonRegionAvailable = ImGui::GetContentRegionAvail();
      const ImVec2 expandButtonCursorPos = ImGui::GetCursorPos();

      ImGui::SetCursorPosX(expandButtonCursorPos.x + expandButtonRegionAvailable.x -
                           (expandAllSize.x + ImGui::GetStyle().ItemSpacing.x + collapseAllSize.x));
      if (ImGui::Button(expandAllTitle))
        favoritesTree.expandAll(true);

      ImGui::SameLine();
      ImGui::SetCursorPosX(expandButtonCursorPos.x + expandButtonRegionAvailable.x - collapseAllSize.x);
      if (ImGui::Button(collapseAllTitle))
        favoritesTree.expandAll(false);
    }

    if (!settingsPanelOpen && ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_F))
      PropPanel::focus_helper.requestFocus(&searchInputFocusId);

    favoritesTree.updateImgui();
  }

private:
  void onShownTypeFilterChanged()
  {
    favoritesTree.setShownTypes(shownTypes);
    favoritesTree.fillTree();
  }

  void showSettingsPanel(const char *popup_id)
  {
    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::GetStyleColorVec4(ImGuiCol_WindowBg));
    const bool popupIsOpen = ImGui::BeginPopup(popup_id, ImGuiWindowFlags_NoMove);
    ImGui::PopStyleColor();

    if (!popupIsOpen)
    {
      settingsPanelOpen = false;
      return;
    }

    ImGui::TextUnformatted("Select filter");

    if (assetTypeFilterControl.updateImgui(assetMgr, make_span(allowedTypes), make_span(shownTypes)))
      onShownTypeFilterChanged();

    ImGui::NewLine();
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 0.0f)); // Leave some space between the separator and the check box.

    bool showHierarchy = favoritesTree.getShowHierarchy();
    const bool showHierarchyPressed = PropPanel::ImguiHelper::checkboxWithDragSelection("Show hierarchy", &showHierarchy);
    PropPanel::set_previous_imgui_control_tooltip((const void *)ImGui::GetItemID(),
      "Show the favorite assets including their hierarchy.");

    if (showHierarchyPressed)
      favoritesTree.setShowHierarchy(showHierarchy);

    ImGui::EndPopup();
  }

  const DagorAssetMgr &assetMgr;
  FavoritesTree favoritesTree;
  AssetTypeFilterControl assetTypeFilterControl;
  dag::Vector<bool> allowedTypes;
  dag::Vector<bool> shownTypes;
  String textToSearch;
  ImTextureID closeIcon = nullptr;
  ImTextureID searchIcon = nullptr;
  ImTextureID settingsIcon = nullptr;
  ImTextureID settingsOpenIcon = nullptr;
  const bool searchInputFocusId = false; // Only the address of this member is used.
  bool settingsPanelOpen = false;
};