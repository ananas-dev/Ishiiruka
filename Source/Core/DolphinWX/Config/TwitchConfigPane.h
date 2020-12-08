// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <wx/arrstr.h>
#include <wx/panel.h>
#include <wx/textctrl.h>

enum TEXIDevices : int;

class wxButton;
class wxCheckBox;
class wxChoice;
class wxDirPickerCtrl;
class wxSpinCtrl;
class wxString;
class wxStaticText;
class wxTextCtrl;

class TwitchConfigPane final : public wxPanel
{
public:
	TwitchConfigPane(wxWindow* parent, wxWindowID id);

private:
	void InitializeGUI();
	void LoadGUIValues();
	void BindEvents();

	void OnTwitchViewersToggle(wxCommandEvent& event);
	void OnStreamUrlChanged(wxCommandEvent& event);

	wxCheckBox* m_twitch_viewers_enable_checkbox;
	wxTextCtrl* m_twitch_stream_url_ctrl;
};
