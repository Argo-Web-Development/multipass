/*
 * Copyright (C) 2019 Canonical, Ltd.
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

#include <multipass/logging/log.h>
#include <multipass/process.h>

namespace mp = multipass;
namespace mpl = multipass::logging;

mp::Process::Process(multipass::logging::Level log_level)
{
    // TODO: multiline output produces poor formatting in logs, needs improving
    QObject::connect(this, &Process::readyReadStandardError, [this, log_level]() {
        mpl::log(log_level, qPrintable(program()), qPrintable(readAllStandardError()));
    });
}

void mp::Process::start(const QStringList& extra_arguments)
{
    QProcess::start(program(), arguments() << extra_arguments);
}

bool mp::Process::run_and_return_status(const QStringList& extra_arguments, const int timeout)
{
    run_and_wait_until_finished(extra_arguments, timeout);
    return exitStatus() == QProcess::NormalExit && exitCode() == 0;
}

QString mp::Process::run_and_return_output(const QStringList& extra_arguments, const int timeout)
{
    run_and_wait_until_finished(extra_arguments, timeout);
    return readAllStandardOutput().trimmed();
}

void mp::Process::run_and_wait_until_finished(const QStringList& extra_arguments, const int timeout)
{
    start(extra_arguments);
    if (!waitForFinished(timeout) || exitStatus() != QProcess::NormalExit)
    {
        mpl::log(mpl::Level::error, qPrintable(program()), qPrintable(errorString()));
    }
}
