#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "simple.h"

using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::Data::Xml::Dom;
using namespace winrt::Windows::UI::Notifications;

namespace winrt::BlankApp1::implementation
{
    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainPage::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        test();

        /*
        XmlDocument doc;
        doc.LoadXml(L"\
        <toast scenario=\"rareAchievement\">\
            <visual>\
                <binding template=\"ToastGeneric\">\
                    <text>Hey this is a line</text>\
                    <text>... and this another ...</text>\
                </binding>\
            </visual>\
        </toast>");

        // Populate with text and values

        // Construct the notification
        winrt::Windows::UI::Notifications::ToastNotification notif{ doc };

        winrt::Windows::UI::Notifications::ToastNotificationManager toastManager{};

        toastManager.ConfigureNotificationMirroring(NotificationMirroring::Allowed);
        //ToastNotifier toastNotifier2 = toastManager.CreateToastNotifier(L"Andromeda_cw5n1h2txyewy!App");
        //ToastNotifier toastNotifier2 = toastManager.CreateToastNotifier(L"XboxOneSystemToasts!Windows.Xbox.SystemToasts.Achievements");
        ToastNotificationManagerForUser mgr2 = toastManager.GetDefault();
        ToastNotifier toastNotifier2 = mgr2.CreateToastNotifier();
        //ToastNotifier toastNotifier2 = mgr2.CreateToastNotifier(L"XboxOneSystemToasts!Windows.Xbox.SystemToasts.Achievements");
        //ToastNotifier toastNotifier2 = toastManager.CreateToastNotifier(L"Windows.Xbox.SystemToasts.Achievements");
        auto setting = toastNotifier2.Setting();

        // And show it!
        toastNotifier2.Show(notif);
        */
    }
}
