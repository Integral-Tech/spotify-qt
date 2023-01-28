#pragma once

#include "lib/enum/followtype.hpp"
#include "lib/qt/httpclient.hpp"
#include "lib/settings.hpp"
#include "lib/spotify/api.hpp"
#include "lib/spotify/artist.hpp"
#include "lib/spotify/device.hpp"
#include "lib/spotify/playback.hpp"
#include "lib/spotify/playlist.hpp"
#include "lib/spotify/searchresults.hpp"
#include "lib/spotify/user.hpp"
#include "lib/strings.hpp"
#include "thirdparty/json.hpp"

#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcessEnvironment>
#include <QSettings>
#include <QString>
#include <QVector>
#include <QtNetwork>
#include <QMessageBox>

namespace spt
{
	class Spotify: public QObject, public lib::spt::api
	{
	Q_OBJECT

	public:
		Spotify(lib::settings &settings, const lib::http_client &httpClient,
			lib::spt::request &request, QObject *parent = nullptr);
	};
}
