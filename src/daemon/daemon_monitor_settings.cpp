/*
 * Copyright (C) 2017-2019 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "daemon_monitor_settings.h"

#include <multipass/settings.h>
#include <multipass/utils.h>

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QObject>

namespace mp = multipass;

namespace
{
constexpr const int settings_changed_code = 42;
void init_settings(const QString& filename)
{
    mp::utils::make_dir({}, QFileInfo{filename}.dir().path()); // make sure parent dir is there
    QFile file{filename};
    file.open(QIODevice::WriteOnly | QIODevice::Append);
}
} // namespace

void multipass::monitor_and_quit_on_settings_change() // temporary
{
    static const auto filename = mp::Settings::get_daemon_settings_file_path();
    init_settings(filename); // create if not there

    static QFileSystemWatcher monitor{{filename}};
    QObject::connect(&monitor, &QFileSystemWatcher::fileChanged, [] { QCoreApplication::exit(settings_changed_code); });
}
