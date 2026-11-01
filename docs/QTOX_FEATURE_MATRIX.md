# qTox → Parley feature matrix

Audit reference: `TokTok/qTox@592c2d3506487c42ffee13e95d67f6efcee2394e`.
Original HAU: `atlasru/hau@73ca3c5d4981ec48df0d9336b8b9778316e9b512`.

**Full parity is not claimed.** `implemented` means an actual backend/action is
present and included in the native build. It does not mean every device/platform
has passed end-to-end testing. `partial`, `missing`, and `unsupported` retain
their literal meaning; independent verification status is in the final column.
All `src/` evidence paths are relative to this repository. Exact test results:
[VERIFICATION.md](VERIFICATION.md). Limitations: [LIMITATIONS.md](LIMITATIONS.md).

| Area | User-facing feature | Status | Source evidence (`src/` unless noted) | Verification / limitation |
| --- | --- | --- | --- | --- |
| Identity | Create a Tox identity | implemented | persistence/profile.cpp | Ported; automated coverage varies; see test report |
| Identity | Import a .tox identity (including password-protected saves) | implemented | widget/tool/profileimporter.cpp | Ported; automated coverage varies; see test report |
| Identity | Export an interoperable identity backup | implemented | model/profile/profileinfo.cpp | Ported; automated coverage varies; see test report |
| Identity | Password set/change/removal and encrypted history | implemented | persistence/profile.cpp | Ported; automated coverage varies; see test report |
| Identity | Profile rename and deletion | implemented | model/profile/profileinfo.cpp | Ported; automated coverage varies; see test report |
| Identity | Concurrent-profile lock protection | implemented | persistence/profilelocker.cpp | Ported; automated coverage varies; see test report |
| Identity | Tox ID copy and QR export | implemented | widget/form/profileform.cpp | Ported; automated coverage varies; see test report |
| Identity | Nickname and status message | implemented | widget/form/profileform.cpp | Ported; automated coverage varies; see test report |
| Identity | Nospam reset/change | implemented | widget/form/profileform.cpp | Ported; automated coverage varies; see test report |
| Identity | Avatar set/remove/resize and broadcast | implemented | net/avatarbroadcaster.cpp | Ported; automated coverage varies; see test report |
| Identity | Online/Away/Busy presence and automatic idle away | implemented | widget/widget.cpp | Ported; automated coverage varies; see test report |
| Identity | Portable mode and explicit profile paths | implemented | persistence/paths.cpp | Ported; automated coverage varies; see test report |
| Identity | Switch accounts without restarting and dispose old Core/AV | implemented | nexus.cpp; appmanager.cpp; test/parley/accounts_test.cpp | Real create/switch/reload regression |
| Identity | Contacts/history/password/settings isolation | implemented | persistence/settings.cpp; persistence/profile.cpp | Real encrypted two-account persistence regression |
| Identity | One-click complete history/files backup and restore | missing | Profile Export is identity-only | Close app and copy entire data directory manually |
| Contacts and chat | Send/accept/reject friend requests and request persistence | implemented | widget/form/addfriendform.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Add by Tox ID or tox: URI | implemented | net/toxuri.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Remove actual contact with confirmation and optional history deletion | implemented | widget/widget.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Contact alias and private note | implemented | model/about/aboutfriend.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Contact circles, rename, move, expand/collapse | implemented | widget/circlewidget.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Contact name/activity/presence search and filters | implemented | model/friendlist/friendlistmanager.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Text and /me action messages | implemented | model/friendmessagedispatcher.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Typing notifications and delivery receipts | implemented | core/core.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Offline outgoing queue / reconnection dispatch | implemented | persistence/offlinemsgengine.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Private SQLCipher message/file history | implemented | persistence/history.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Save/export chat log | implemented | widget/form/genericchatform.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Find forward/backward, case, whole words, regex and date search | implemented | widget/searchform.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Message copy/select all, hyperlinks and context menu | implemented | chatlog/chatwidget.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Emoji packs, text formatting and composition settings | implemented | widget/form/settings/userinterfaceform.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Detached conversation windows and keyboard chat navigation | implemented | widget/contentdialog.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Per-contact auto-accept files/calls/conferences | implemented | widget/about/aboutfriendform.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | View contact details and public key | implemented | widget/about/aboutfriendform.cpp | Ported qTox backend; unit tests + manual checks as listed |
| Contacts and chat | Archive/unarchive (Parley extension) | implemented | parley/chatactions.cpp; persistence/settings.cpp; widget/widget.cpp | Accounts/history regression; UI/manual checks remain |
| Contacts and chat | Pin/unpin and persistent pin order (Parley extension) | implemented | parley/chatactions.cpp; persistence/settings.cpp; widget/widget.cpp | Accounts/history regression; UI/manual checks remain |
| Contacts and chat | Mute/unmute without dropping messages (Parley extension) | implemented | parley/chatactions.cpp; persistence/settings.cpp; widget/widget.cpp | Accounts/history regression; UI/manual checks remain |
| Contacts and chat | Persistent numeric unread counters (Parley extension) | implemented | parley/chatactions.cpp; persistence/settings.cpp; widget/widget.cpp | Accounts/history regression; UI/manual checks remain |
| Contacts and chat | Delete private chat + local history, retaining contact | implemented | parley/chatactions.cpp; persistence/settings.cpp; widget/widget.cpp | Accounts/history regression; UI/manual checks remain |
| Contacts and chat | Clear private history without contact removal | implemented | parley/chatactions.cpp; persistence/settings.cpp; widget/widget.cpp | Accounts/history regression; UI/manual checks remain |
| Contacts and chat | Manual drag reordering of pinned chats | missing | Pin sequence is stable; existing circle drag/drop retained | Not exposed as a fake action |
| Legacy Tox conferences | Create text and AV conferences | implemented | core/core.cpp | Ported; localhost conference transport test; hardware/manual pending |
| Legacy Tox conferences | Conference invitations, accept/reject and auto-accept | implemented | widget/form/conferenceinviteform.cpp | Ported; localhost conference transport test; hardware/manual pending |
| Legacy Tox conferences | Conference messaging, /me and mentions | implemented | model/conferencemessagedispatcher.cpp | Ported; localhost conference transport test; hardware/manual pending |
| Legacy Tox conferences | Peer list, name completion and peer count | implemented | widget/form/conferenceform.cpp | Ported; localhost conference transport test; hardware/manual pending |
| Legacy Tox conferences | Change conference title and leave conference | implemented | widget/conferencewidget.cpp | Ported; localhost conference transport test; hardware/manual pending |
| Legacy Tox conferences | Conference audio call and peer mute/volume | implemented | widget/form/conferenceform.cpp | Ported; localhost conference transport test; hardware/manual pending |
| Legacy Tox conferences | Conference join/leave visibility and mention notification options | implemented | widget/form/settings/userinterfaceform.cpp | Ported; localhost conference transport test; hardware/manual pending |
| Legacy Tox conferences | Persisted conference message history and local purge | implemented | model/chatmanager.cpp; model/chathistory.cpp | Shared SQLCipher history engine; local clear regression |
| Legacy Tox conferences | Conference owner/admin/kick/ban/password roles | unsupported | Legacy Tox conference protocol has no such roles | Modern NGC is separate and missing |
| Modern Tox groups | NGC create/join/roles/moderation/privacy | missing | No NGC group UI or backend in pinned qTox | Do not confuse conferences with new groups |
| Audio/video | Outgoing/incoming audio and video, accept/reject/hang up | implemented | core/coreav.cpp | Ported; codec/frame tests; live device testing pending |
| Audio/video | Microphone and speaker mute; call volume | implemented | video/netcamview.cpp | Ported; codec/frame tests; live device testing pending |
| Audio/video | Camera selection, resolution/FPS and local preview | implemented | widget/form/settings/avform.cpp | Ported; codec/frame tests; live device testing pending |
| Audio/video | Audio input/output device selection and testing | implemented | widget/form/settings/avform.cpp | Ported; codec/frame tests; live device testing pending |
| Audio/video | Microphone gain, noise threshold, output volume, bitrate | implemented | widget/form/settings/avform.cpp | Ported; codec/frame tests; live device testing pending |
| Audio/video | Screen sharing, monitor/region selection | implemented | widget/tool/screenshotgrabber.cpp | Ported; codec/frame tests; live device testing pending |
| Audio/video | Automatic incoming call permissions per contact | implemented | widget/about/aboutfriendform.cpp | Ported; codec/frame tests; live device testing pending |
| Audio/video | Dedicated Calls dashboard / persistent call journal | missing | Calls are available in the conversation header | No placeholder dashboard |
| Files | Send/receive actual files with progress, speed, ETA | implemented | core/corefile.cpp | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | Accept/reject and cancel transfer | implemented | chatlog/content/filetransferwidget.cpp | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | Pause/resume with local and remote pause state | implemented | core/toxfilepause.h | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | Broken transfer handling and reconnection | implemented | core/corefile.cpp | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | File transfer manager, sent/received lists, open files/folders | implemented | widget/form/filesform.cpp | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | File drag-and-drop, clipboard images and screenshots | implemented | widget/form/chatform.cpp | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | Image previews, EXIF orientation and avatar image decoding | implemented | core/untrustedimage.cpp | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | Global/contact-specific auto-accept directory and size cap | implemented | widget/form/settings/generalform.cpp | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | File history records, open/executable confirmation | implemented | persistence/history.cpp | Ported; transfer widget/progress/image tests; live transport checks listed separately |
| Files | Resume every download across process restart/account changes | partial | qTox broken-transfer logic is preserved | Not certified across restart/switch |
| Files | Dedicated cross-chat file/image gallery | missing | Per-chat history and sent/received manager exist | No unified gallery |
| Network/advanced | IPv6, UDP/TCP mode, LAN discovery | implemented | widget/form/settings/advancedform.cpp | Ported; options/bootstrap/schema tests; public-network/lifecycle checks pending |
| Network/advanced | SOCKS5 and HTTP proxy with address and port | implemented | core/toxoptions.cpp | Ported; options/bootstrap/schema tests; public-network/lifecycle checks pending |
| Network/advanced | Default/custom bootstrap nodes and TCP relays | implemented | net/bootstrapnodeupdater.cpp | Ported; options/bootstrap/schema tests; public-network/lifecycle checks pending |
| Network/advanced | Custom DHT server list selection/editor and import | implemented | widget/form/settings/advancedform.cpp | Ported; options/bootstrap/schema tests; public-network/lifecycle checks pending |
| Network/advanced | Bootstrap list update and backup | implemented | net/bootstrapnodeupdater.cpp | Ported; options/bootstrap/schema tests; public-network/lifecycle checks pending |
| Network/advanced | Rebootstrap/reconnect on network loss | implemented | core/core.cpp | Ported; options/bootstrap/schema tests; public-network/lifecycle checks pending |
| Network/advanced | Debug log viewer, object tree and toxcore log level | implemented | widget/form/debugwidget.cpp | Ported; options/bootstrap/schema tests; public-network/lifecycle checks pending |
| Network/advanced | Database synchronous mode tuning and migration/reset controls | implemented | widget/form/settings/advancedform.cpp | Ported; options/bootstrap/schema tests; public-network/lifecycle checks pending |
| Privacy/application | Enable/disable local history and typing | implemented | widget/form/settings/privacyform.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | Blocked conference invite words/user list | implemented | widget/form/settings/privacyform.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | SQLCipher history schema migrations 0 → 11 | implemented | persistence/db/upgrades/dbupgrader.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | Personal settings schema migration 1 → 2 | implemented | persistence/personalsettingsupgrader.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | Atomic .tox and personal settings writes | implemented | persistence/profile.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | Profile import failure cannot erase an existing identity | implemented | widget/tool/profileimporter.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | Desktop notifications, sounds, Busy exceptions, preview hiding | implemented | widget/form/settings/userinterfaceform.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | Tray, close/minimize, startup and translation preferences | implemented | widget/form/settings/generalform.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | Accessibility labels, focus outlines, keyboard shortcuts | implemented | parley/designsystem.cpp | Ported or hardened; automated coverage listed in verification report |
| Privacy/application | Automatic qTox binary update notifications | unsupported | UPDATE_CHECK defaults OFF in Parley | Upstream binary is not a Parley update |
| Privacy/application | Spell checking with KF6 Sonnet | partial | Conditional upstream feature | Dependency unavailable in this Linux build |
| Design | Light/dark/AMOLED desktop themes | implemented | parley/designsystem.cpp; widget/style.cpp | Shared palette tokens + existing theme picker |
| Design | Rounded reusable controls and layered surface gradients | implemented | parley/designsystem.cpp; widget/style.cpp | parley/designsystem.cpp appended to shared Style |
| Design | Desktop system accent choices | implemented | parley/designsystem.cpp; widget/style.cpp | QPalette Highlight via theme picker |
| Design | Optional glass highlights / opaque fallback | implemented | parley/designsystem.cpp; widget/style.cpp | No expensive blur applied to foreground |
| Design | Native Liquid Glass refraction/backdrop blur | missing | parley/designsystem.cpp; widget/style.cpp | Opaque fallback only |
| Design | Full Material 3 Expressive component set | partial | parley/designsystem.cpp; widget/style.cpp | Visual adaptation; Qt Widgets interaction system |
| Design | Android wallpaper Dynamic Color | unsupported | parley/designsystem.cpp; widget/style.cpp | No supported Android frontend |
| Design | Reduced-motion preference | partial | parley/designsystem.cpp; widget/style.cpp | Saved control; surfaces currently have no mandatory animation |
| Design | Spring navigation transitions | missing | parley/designsystem.cpp; widget/style.cpp | Not implemented |
| Design | Mobile adaptive conversation/navigation layout | missing | parley/designsystem.cpp; widget/style.cpp | Desktop split panes retained |
| Design | Dedicated Calls/account switcher/gallery screens | partial | parley/designsystem.cpp; widget/style.cpp | Existing functional dialogs/headers, no complete screen redesign |

## Complete reference Designer control inventory

Generated from every user-facing `.ui` file in the **unmodified pinned reference**,
not from screenshots or a README feature list. This preserves advanced controls
that would otherwise be easy to miss. Dynamic menus and code-created controls
are inventoried separately below. Labels/group containers are omitted here;
their interactive child controls remain listed. A ported control inherits the
status of its related row above, including dependency/platform restrictions.

| Reference form | Interactive control | Label / tooltip | Port status |
| --- | --- | --- | --- |
| `src/chatlog/content/filetransferwidget.ui` | `rightButton` (QPushButton) |  | implemented (ported) |
| `src/chatlog/content/filetransferwidget.ui` | `leftButton` (QPushButton) |  | implemented (ported) |
| `src/loginscreen.ui` | `newUsername` (QLineEdit) |  | implemented (ported) |
| `src/loginscreen.ui` | `createAccountButton` (QPushButton) | Create Profile | implemented (ported) |
| `src/loginscreen.ui` | `loginUsernames` (QComboBox) | Profile list | implemented (ported) |
| `src/loginscreen.ui` | `autoLoginCB` (QCheckBox) | Load automatically | implemented (ported) |
| `src/loginscreen.ui` | `importButton` (QPushButton) | Import | implemented (ported) |
| `src/loginscreen.ui` | `loginButton` (QPushButton) | Load | implemented (ported) |
| `src/loginscreen.ui` | `newProfilePgbtn` (QPushButton) | New Profile | implemented (ported) |
| `src/loginscreen.ui` | `loginPgbtn` (QPushButton) | Load Profile | implemented (ported) |
| `src/mainwindow.ui` | `statusButton` (QPushButton) | Status | implemented (ported) |
| `src/mainwindow.ui` | `searchContactText` (QLineEdit) | Contact search | implemented (ported) |
| `src/mainwindow.ui` | `searchContactFilterBox` (QToolButton) | ... | implemented (ported) |
| `src/mainwindow.ui` | `addButton` (QPushButton) | Add friend | implemented (ported) |
| `src/mainwindow.ui` | `conferenceButton` (QPushButton) | Create a conference | implemented (ported) |
| `src/mainwindow.ui` | `transferButton` (QPushButton) | View completed file transfers | implemented (ported) |
| `src/mainwindow.ui` | `settingsButton` (QPushButton) | Change your settings | implemented (ported) |
| `src/mainwindow.ui` | `debugButton` (QPushButton) | Open internal debugging tools | implemented (ported) |
| `src/widget/about/aboutfriendform.ui` | `publicKey` (QLineEdit) |  | implemented (ported) |
| `src/widget/about/aboutfriendform.ui` | `autoacceptcall` (QComboBox) |  | implemented (ported) |
| `src/widget/about/aboutfriendform.ui` | `autoConferenceInvite` (QCheckBox) | Auto-accept conference invites | implemented (ported) |
| `src/widget/about/aboutfriendform.ui` | `autoacceptfile` (QCheckBox) | Auto-accept files | implemented (ported) |
| `src/widget/about/aboutfriendform.ui` | `selectSaveDir` (QPushButton) | Auto-accept for this contact is disabled | implemented (ported) |
| `src/widget/about/aboutfriendform.ui` | `removeHistory` (QPushButton) | Remove history (operation can not be undone!) | implemented (ported) |
| `src/widget/about/aboutfriendform.ui` | `note` (QPlainTextEdit) |  | implemented (ported) |
| `src/widget/form/debug/debuglog.ui` | `cbAutoReload` (QCheckBox) | Auto-reload | implemented (ported) |
| `src/widget/form/debug/debuglog.ui` | `cbAutoScroll` (QCheckBox) | Auto-scroll | implemented (ported) |
| `src/widget/form/debug/debuglog.ui` | `cmbFilters` (QComboBox) |  | implemented (ported) |
| `src/widget/form/debug/debugobjecttree.ui` | `btnReload` (QPushButton) | Reload | implemented (ported) |
| `src/widget/form/profileform.ui` | `userName` (QLineEdit) | Name input | implemented (ported) |
| `src/widget/form/profileform.ui` | `statusMessage` (QLineEdit) | Status message input | implemented (ported) |
| `src/widget/form/profileform.ui` | `saveQr` (QPushButton) | Save image | implemented (ported) |
| `src/widget/form/profileform.ui` | `copyQr` (QPushButton) | Copy image | implemented (ported) |
| `src/widget/form/profileform.ui` | `renameButton` (QPushButton) | Rename | implemented (ported) |
| `src/widget/form/profileform.ui` | `deleteButton` (QPushButton) | Delete | implemented (ported) |
| `src/widget/form/profileform.ui` | `exportButton` (QPushButton) | Export | implemented (ported) |
| `src/widget/form/profileform.ui` | `logoutButton` (QPushButton) | Logout | implemented (ported) |
| `src/widget/form/profileform.ui` | `deletePassButton` (QPushButton) | Remove password | implemented (ported) |
| `src/widget/form/profileform.ui` | `changePassButton` (QPushButton) | Change password | implemented (ported) |
| `src/widget/form/removechatdialog.ui` | `removeHistory` (QCheckBox) | Also remove chat history | implemented (ported) |
| `src/widget/form/searchsettingsform.ui` | `startSearchComboBox` (QComboBox) |  | implemented (ported) |
| `src/widget/form/searchsettingsform.ui` | `choiceDateButton` (QPushButton) |  | implemented (ported) |
| `src/widget/form/searchsettingsform.ui` | `registerCheckBox` (QCheckBox) | Case sensitive | implemented (ported) |
| `src/widget/form/searchsettingsform.ui` | `wordsOnlyRadioButton` (QRadioButton) | Whole words only | implemented (ported) |
| `src/widget/form/searchsettingsform.ui` | `regularRadioButton` (QRadioButton) | Use regular expressions | implemented (ported) |
| `src/widget/form/settings/aboutsettings.ui` | `updateAvailableButton` (QPushButton) | Update available | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `cbMakeToxPortable` (QCheckBox) | Make Tox portable | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `btnCopyDebug` (QPushButton) | Copy Debug Log | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `btnExportLog` (QPushButton) | Export Debug Log | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `cbEnableDebug` (QCheckBox) | Enable Debug Tools (developers only) | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `cbLogToFile` (QCheckBox) | Write debug log to file | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `toxcoreLogLevel` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `cbEnableIPv6` (QCheckBox) | Enable IPv6 (recommended) | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `cbEnableUDP` (QCheckBox) | Enable UDP (recommended) | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `cbEnableLanDiscovery` (QCheckBox) | Enable LAN discovery | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `proxyType` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `proxyAddr` (QLineEdit) |  | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `proxyPort` (QSpinBox) |  | implemented (ported) |
| `src/widget/form/settings/advancedsettings.ui` | `resetButton` (QPushButton) | Reset to default settings | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `cbEnableTestSound` (QCheckBox) | Test Sound | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `playbackSlider` (QSlider) | Use slider to set the volume of your speakers. | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `inDevCombobox` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `outDevCombobox` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `microphoneSlider` (QSlider) |  | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `audioThresholdSlider` (QSlider) |  | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `audioQualityComboBox` (QComboBox) | Transmitted audio quality. Lower this setting if your bandwidth is not high enough or if you want to reduce bandwidth usage. | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `videoModesComboBox` (QComboBox) | Set resolution of your camera. The higher values, the better video quality your friends may get. Note that with better video quality, you use more bandwidth. Sometimes your connection may not be good enough to handle higher video quality, which may lead to problems with video calls. | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `videoDevCombobox` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/avform.ui` | `rescanButton` (QPushButton) | Rescan devices | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `transComboBox` (QComboBox) | The translation may not load until qTox restarts. | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `cbAutorun` (QCheckBox) | Autostart | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `checkUpdates` (QCheckBox) | Check for updates | unsupported (fork updater disabled) |
| `src/widget/form/settings/generalsettings.ui` | `cbSpellChecking` (QCheckBox) | Spell checking | partial (Sonnet dependency) |
| `src/widget/form/settings/generalsettings.ui` | `lightTrayIcon` (QCheckBox) | Light icon | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `showSystemTray` (QCheckBox) | Show system tray icon | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `startInTray` (QCheckBox) | Start in tray | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `minimizeToTray` (QCheckBox) | Minimize to tray | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `closeToTray` (QCheckBox) | Close to tray | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `statusChanges` (QCheckBox) | Show contacts' status changes | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `conferenceJoinLeaveMessages` (QCheckBox) | Add a chat message when a user joins or leaves a conference | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `autoAwaySpinBox` (QSpinBox) | Set to 0 to disable | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `autoSaveFilesDir` (QPushButton) | Set where files will be saved. | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `autoacceptFiles` (QCheckBox) | Autoaccept files | implemented (ported) |
| `src/widget/form/settings/generalsettings.ui` | `maxAutoAcceptSizeMB` (QDoubleSpinBox) |  | implemented (ported) |
| `src/widget/form/settings/privacysettings.ui` | `cbTypingNotification` (QCheckBox) | Send typing notifications | implemented (ported) |
| `src/widget/form/settings/privacysettings.ui` | `cbKeepHistory` (QCheckBox) | Keep chat history | implemented (ported) |
| `src/widget/form/settings/privacysettings.ui` | `nospamLineEdit` (QLineEdit) |  | implemented (ported) |
| `src/widget/form/settings/privacysettings.ui` | `randomNospamButton` (QPushButton) | Generate random NoSpam | implemented (ported) |
| `src/widget/form/settings/privacysettings.ui` | `blockListTextEdit` (QTextEdit) | Filter out conference messages by conference members' public keys. Put public keys here, one per line. | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `txtChatFont` (QFontComboBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `txtChatFontSize` (QSpinBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `textStyleComboBox` (QComboBox) | Select text styling preference. | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `useNameColors` (QCheckBox) | Use colored nicknames in conferences | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `cbImagePreview` (QCheckBox) | Image preview | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `chatLogMaxTxt` (QSpinBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `chatLogChunkTxt` (QSpinBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `cbHidePostNullSuffix` (QCheckBox) | Hide suffix after NULL symbol | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `notify` (QCheckBox) | Notify | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `conferenceOnlyNotifyWhenMentioned` (QCheckBox) | Conferences only notify when mentioned | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `notifySound` (QCheckBox) | Play sound | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `busySound` (QCheckBox) | Play sound while Busy | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `desktopNotify` (QCheckBox) | Notify via desktop notifications | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `notifySystemBackend` (QCheckBox) | Use system-specific notification backend if available | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `notifyHide` (QCheckBox) | Hide message sender and contents | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `showWindow` (QCheckBox) | Open window | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `cbConferencePosition` (QCheckBox) | Place conferences at top of friend list | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `cbCompactLayout` (QCheckBox) | Compact contact list | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `cbSeparateWindow` (QCheckBox) | Multiple windows mode | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `cbDontGroupWindows` (QCheckBox) | Open each chat in an individual window | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `cbShowIdenticons` (QCheckBox) | Use identicons instead of empty avatars | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `useEmoticons` (QCheckBox) | Use emoticons | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `smileyPackBrowser` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `emoticonSize` (QSpinBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `styleBrowser` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `themeColorCBox` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `timestamp` (QComboBox) |  | implemented (ported) |
| `src/widget/form/settings/userinterfacesettings.ui` | `dateFormats` (QComboBox) |  | implemented (ported) |

## Reference dynamic menu/action inventory

Generated from code-created `tr(...)` action/menu entries in the reference.
Duplicates are grouped by literal label. Entries are preserved in the imported
source; protocol/platform and update-check restrictions above still apply.

| Action/menu label | Reference source | Port status |
| --- | --- | --- |
| Auto accept files from this friend | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Create new circle... | `src/widget/widget.cpp` | implemented (ported; platform conditions apply) |
| Create new conference... | `src/widget/widget.cpp` | implemented (ported; platform conditions apply) |
| Invite to conference | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Invite to conference '%1' | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Move to circle... | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Open all in new window | `src/widget/circlewidget.cpp` | implemented (ported; platform conditions apply) |
| Open chat in new window | `src/widget/conferencewidget.cpp`, `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Quit conference | `src/widget/conferencewidget.cpp` | implemented (ported; platform conditions apply) |
| Remove chat from this window | `src/widget/conferencewidget.cpp`, `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Remove circle | `src/widget/circlewidget.cpp` | implemented (ported; platform conditions apply) |
| Remove friend | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Remove from circle '%1' | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Rename circle | `src/widget/circlewidget.cpp` | implemented (ported; platform conditions apply) |
| Set alias... | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| Set title... | `src/widget/conferencewidget.cpp` | implemented (ported; platform conditions apply) |
| Show details | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| To new circle | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| To new conference | `src/widget/friendwidget.cpp` | implemented (ported; platform conditions apply) |
| copy peer ID | `src/widget/form/conferenceform.cpp` | implemented (ported; platform conditions apply) |

## Platform support

| Platform | Status | Artifact/test claim |
| --- | --- | --- |
| Linux x86_64 / Qt 6.4 | implemented | Actual native build; exact results in VERIFICATION.md |
| Windows x64 / Qt6 UCRT64 | partial | CI/build recipe supplied; no successful Windows artifact in this environment |
| Android / iOS | unsupported | No supported artifact; inherited experimental platform files do not change this |
| macOS / Web / Haiku | unsupported | Upstream files retained; no Parley validation |
