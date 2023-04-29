#pragma once

#include "view/context/title.hpp"
#include "view/context/expandedcontent.hpp"
#include "view/context/abstractcontent.hpp"
#include "view/context/smallcontent.hpp"
#include "lib/settings.hpp"
#include "lib/enum/albumsize.hpp"

#include <QDockWidget>

namespace Context
{
	class View: public QDockWidget
	{
	Q_OBJECT

	public:
		View(lib::spt::api &spotify, lib::settings &settings, spt::Current &current,
			const lib::cache &cache, QWidget *parent);

		void setAlbum(const lib::spt::entity &albumEntity, const QPixmap &albumImage) const;
		void setAlbumSize(lib::album_size albumSize);

	private:
		lib::spt::api &spotify;
		spt::Current &current;
		const lib::cache &cache;

		Title *title = nullptr;
		AbstractContent *albumContent = nullptr;

		void onPlaybackRefreshed(const lib::spt::playback &playback,
			const lib::spt::playback &previous);
	};
}
