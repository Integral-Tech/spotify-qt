#include "server.hpp"
#include "lib/log.hpp"
#include "commandline/args.hpp"
#include "mainwindow.hpp"

Ipc::Server::Server(lib::spt::api &spotify, QObject *parent)
	: QLocalServer(parent),
	spotify(spotify)
{
	setMaxPendingConnections(1);
}

auto Ipc::Server::start() -> bool
{
	if (!listen(APP_NAME))
	{
		lib::log::error("Failed to start IPC server: {}",
			errorString().toStdString());
		return false;
	}

	QLocalServer::connect(this, &QLocalServer::newConnection,
		this, &Ipc::Server::onNewConnection);

	return true;
}

void Ipc::Server::onNewConnection()
{
	socket = nextPendingConnection();
	QLocalSocket::connect(socket, &QLocalSocket::disconnected,
		socket, &QLocalSocket::deleteLater);

	stream.setDevice(socket);

	QLocalSocket::connect(socket, &QLocalSocket::readyRead,
		this, &Ipc::Server::onReadyRead);
}

void Ipc::Server::onReadyRead()
{
	if (!stream.atEnd())
	{
		return;
	}

	QByteArray buffer;
	QDataStream out(&buffer, QIODevice::WriteOnly);

	QString data;
	stream >> data;

	out << (onReadAll(data)
		? QStringLiteral("ok")
		: QStringLiteral("err"));

	socket->write(buffer);
	socket->flush();
	socket->disconnectFromServer();
}

void Ipc::Server::logStatus(const QString &data, const std::string &status)
{
	if (status.empty())
	{
		return;
	}

	lib::log::warn("{} failed: {}", data.toStdString(), status);
}

auto Ipc::Server::onReadAll(const QString &data) -> bool
{
	auto *mainWindow = qobject_cast<MainWindow *>(parent());
	if (mainWindow == nullptr)
	{
		lib::log::warn("{} failed: no window", ARG_PLAY_PAUSE.toStdString());
		return false;
	}

	if (data == ARG_PLAY_PAUSE)
	{
		auto callback = [data](const std::string &status)
		{
			Ipc::Server::logStatus(data, status);
		};

		if (mainWindow->currentPlayback().is_playing)
		{
			spotify.pause(callback);
		}
		else
		{
			spotify.resume(callback);
		}

		return true;
	}

	if (data == ARG_PREVIOUS_TRACK)
	{
		spotify.previous([data](const std::string &status)
		{
			Ipc::Server::logStatus(data, status);
		});
		return true;
	}

	if (data == ARG_NEXT_TRACK)
	{
		spotify.next([data](const std::string &status)
		{
			Ipc::Server::logStatus(data, status);
		});
		return true;
	}

	lib::log::warn("Unrecognized command: {}", data.toStdString());
	return false;
}