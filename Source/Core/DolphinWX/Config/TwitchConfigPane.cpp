// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "DolphinWX/Config/TwitchConfigPane.h"

#include <cassert>
#include <string>

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/filepicker.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>

#include "Common/Common.h"
#include "Common/CommonPaths.h"
#include "Common/FileUtil.h"
#include "Core/ConfigManager.h"
#include "Core/Core.h"
#include "Core/HW/EXI.h"
#include "Core/HW/GCMemcard.h"
#include "Core/HW/GCPad.h"
#include "Core/NetPlayProto.h"
#include "DolphinWX/Config/ConfigMain.h"
#include "DolphinWX/Input/MicButtonConfigDiag.h"
#include "DolphinWX/WxEventUtils.h"
#include "DolphinWX/WxUtils.h"

TwitchConfigPane::TwitchConfigPane(wxWindow* parent, wxWindowID id) : wxPanel(parent, id)
{
	InitializeGUI();
	LoadGUIValues();
	BindEvents();
}

void TwitchConfigPane::InitializeGUI()
{

#ifndef IS_PLAYBACK
	// Twitch Settings
	m_twitch_viewers_enable_checkbox = new wxCheckBox(this, wxID_ANY, _("Enable Twitch Viewers"));
	m_twitch_viewers_enable_checkbox->SetToolTip(
		_("Enable having your Twitch viewers number displayed on the side of the emulation window."));

	m_twitch_stream_url_ctrl = new wxTextCtrl(this, wxID_ANY, _("Stream URL:"), wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, wxTextCtrlNameStr);
	m_twitch_stream_url_ctrl->SetToolTip(_("Enter the URL of the Twitch stream you want to get the viewers from. Example: https://www.twitch.tv/btssmash"));

#endif
	const int space5 = FromDIP(5);
	const int space10 = FromDIP(10);

#ifndef IS_PLAYBACK
	wxGridBagSizer* const sTwitchSettings = new wxGridBagSizer(space5, space5);
	sTwitchSettings->Add(m_twitch_viewers_enable_checkbox, wxGBPosition(0, 0), wxGBSpan(1, 2));
	sTwitchSettings->Add(new wxStaticText(this, wxID_ANY, _("Stream Url:")), wxGBPosition(1, 0),
		wxDefaultSpan, wxALIGN_CENTER_VERTICAL);
	sTwitchSettings->Add(m_twitch_stream_url_ctrl, wxGBPosition(1, 1), wxDefaultSpan, wxEXPAND);
	sTwitchSettings->AddGrowableCol(1);

	wxStaticBoxSizer* const sbTwitchReplaySettings =
		new wxStaticBoxSizer(wxVERTICAL, this, _("Twitch Settings"));
	sbTwitchReplaySettings->AddSpacer(space5);
	sbTwitchReplaySettings->Add(sTwitchSettings, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	sbTwitchReplaySettings->AddSpacer(space5);

#endif
	wxBoxSizer* const main_sizer = new wxBoxSizer(wxVERTICAL);
#ifndef IS_PLAYBACK
	main_sizer->AddSpacer(space5);
	main_sizer->Add(sbTwitchReplaySettings, 0, wxEXPAND | wxLEFT | wxRIGHT, space5);
	main_sizer->AddSpacer(space5);
#endif
	SetSizer(main_sizer);
}
void TwitchConfigPane::LoadGUIValues()
{
	const SConfig& startup_params = SConfig::GetInstance();

#if HAVE_PORTAUDIO
#endif


#ifndef IS_PLAYBACK

	m_twitch_viewers_enable_checkbox->SetValue(startup_params.m_twitchEnableViewerCounter);
	m_twitch_stream_url_ctrl->SetValue(StrToWxStr(startup_params.m_strTwitchStreamUrl));

#endif
}

void TwitchConfigPane::BindEvents()
{
#ifndef IS_PLAYBACK
	m_twitch_viewers_enable_checkbox->Bind(wxEVT_CHECKBOX, &TwitchConfigPane::OnTwitchViewersToggle, this);
	m_twitch_stream_url_ctrl->Bind(wxEVT_TEXT, &TwitchConfigPane::OnStreamUrlChanged, this);
#endif
}


void TwitchConfigPane::OnTwitchViewersToggle(wxCommandEvent& event)
{
	bool enableViewerCounter = m_twitch_viewers_enable_checkbox->IsChecked();
	SConfig::GetInstance().m_twitchEnableViewerCounter = enableViewerCounter;
}


void TwitchConfigPane::OnStreamUrlChanged(wxCommandEvent& event)
{
	wxString str = m_twitch_stream_url_ctrl->GetValue();
	SConfig::GetInstance().m_strTwitchStreamUrl = WxStrToStr(str);
}