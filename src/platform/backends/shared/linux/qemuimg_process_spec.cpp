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

#include "qemuimg_process_spec.h"

#include <multipass/snap_utils.h>

namespace mp = multipass;
namespace mu = multipass::utils;

mp::QemuImgProcessSpec::QemuImgProcessSpec(const QStringList& args) : args{args}
{
}

QString mp::QemuImgProcessSpec::program() const
{
    return "qemu-img";
}

QStringList mp::QemuImgProcessSpec::arguments() const
{
    return args;
}

QString mp::QemuImgProcessSpec::apparmor_profile() const
{
    QString profile_template(R"END(
#include <tunables/global>
profile %1 flags=(attach_disconnected) {
  #include <abstractions/base>

  %2

  # binary and its libs
  %3/usr/bin/qemu-img ixr,
  %3/{usr/,}lib/** rm,

  # Subdirectory containing disk image(s)
  %4/** rwk,
}
    )END");

    QString extra_capabilities;
    if (!mu::is_snap())
    {
        // FIXME - unclear why this is required when not snap confined
        extra_capabilities = "capability dac_read_search,\n    capability dac_override,";
    }

    return profile_template.arg(apparmor_profile_name(), extra_capabilities, mu::snap_dir(), mu::snap_common_dir());
}
