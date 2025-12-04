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


// Check for C++17 support.
#if __cplusplus < 201703L && !defined(_MSVC_LANG)
#error "This library requires C++17 or later."
#endif
#if defined(_MSVC_LANG) && _MSVC_LANG < 201703L
#error "This library requires C++17 or later."
#endif


// Check for constexpr support.
#if __has_feature(constexpr)
#define IF_CONSTEXPR if constexpr
#else
#define IF_CONSTEXPR if
#endif
