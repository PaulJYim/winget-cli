// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#pragma once
#include "SQLiteWrapper.h"
#include "Manifest/Manifest.h"
#include "Microsoft/Schema/Version.h"
#include "Public/AppInstallerRepositorySearch.h"

#include <filesystem>


namespace AppInstaller::Repository::Microsoft::Schema
{
    // Forward declarations
    struct Version;

    // The common interface used to interact with all schema versions of the index.
    struct ISQLiteIndex
    {
        virtual ~ISQLiteIndex() = default;

        // Version 1.0

        // Gets the schema version that this index interface is built for.
        virtual Schema::Version GetVersion() const = 0;

        // Creates all of the version dependent tables within the database.
        virtual void CreateTables(SQLite::Connection& connection) = 0;

        // Adds the manifest at the repository relative path to the index.
        virtual void AddManifest(SQLite::Connection& connection, const Manifest::Manifest& manifest, const std::filesystem::path& relativePath) = 0;

        // Updates the manifest with matching { Id, Version, Channel } in the index.
        // The return value indicates whether the index was modified by the function.
        virtual bool UpdateManifest(SQLite::Connection& connection, const Manifest::Manifest& manifest, const std::filesystem::path& relativePath) = 0;

        // Removes the manifest with matching { Id, Version, Channel } from the index.
        // Path is currently ignored.
        virtual void RemoveManifest(SQLite::Connection& connection, const Manifest::Manifest& manifest, const std::filesystem::path& relativePath) = 0;

        // Removes data that is no longer needed for an index that is to be published.
        virtual void PrepareForPackaging(SQLite::Connection& connection) = 0;

        // Performs a search based on the given criteria.
        virtual std::vector<std::pair<SQLite::rowid_t, ApplicationMatchFilter>> Search(SQLite::Connection& connection, const SearchRequest& request) = 0;

        // Gets the Id string for the given id, if present.
        virtual std::optional<std::string> GetIdStringById(SQLite::Connection& connection, SQLite::rowid_t id) = 0;

        // Gets the Name string for the given id, if present.
        virtual std::optional<std::string> GetNameStringById(SQLite::Connection& connection, SQLite::rowid_t id) = 0;

        // Gets the relative path string for the given { id, version, channel }, if present.
        // If version is empty, gets the value for the 'latest' version.
        virtual std::optional<std::string> GetPathStringByKey(SQLite::Connection& connection, SQLite::rowid_t id, std::string_view version, std::string_view channel) = 0;

        // Gets all versions and channels for the given id.
        virtual std::vector<std::pair<std::string, std::string>> GetVersionsById(SQLite::Connection& connection, SQLite::rowid_t id) = 0;
    };


    // Common base class used by all schema versions.
    struct SQLiteIndexBase : public ISQLiteIndex
    {
    };
}