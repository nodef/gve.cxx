// Copyright (C) 2025 Subhajit Sahu
// SPDX-License-Identifier: AGPL-3.0-or-later
// See LICENSE for full terms
#pragma once

// Check for module support.
#ifdef _MODULE_
#define EXPORT export
#else
#define EXPORT
#endif


// Check for constexpr support.
#if __has_feature(constexpr)
#define IF_CONSTEXPR if constexpr
#else
#define IF_CONSTEXPR if
#endif
