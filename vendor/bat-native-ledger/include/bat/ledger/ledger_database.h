/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_LEDGER_LEDGER_DATABASE_H_
#define BAT_LEDGER_LEDGER_DATABASE_H_

#include <string>
#include <memory>

#include "base/files/file_path.h"
#include "base/memory/memory_pressure_listener.h"
#include "base/sequence_checker.h"
#include "bat/ledger/ledger_client.h"
#include "sql/database.h"
#include "sql/init_status.h"
#include "sql/meta_table.h"

namespace ledger {

class LEDGER_EXPORT LedgerDatabase {
 public:
  explicit LedgerDatabase(const std::string& path);

  LedgerDatabase(const LedgerDatabase&) = delete;
  LedgerDatabase& operator=(const LedgerDatabase&) = delete;

  ~LedgerDatabase();

  void RunTransaction(
      DBTransactionPtr transaction,
      DBCommandResponse* command_response);

 private:
  DBCommandResponse::Status Initialize(
      int32_t version,
      int32_t compatible_version,
      DBCommandResponse* command_response);

  DBCommandResponse::Status Execute(DBCommand* command);

  DBCommandResponse::Status Run(DBCommand* command);

  DBCommandResponse::Status Read(
      DBCommand* command,
      DBCommandResponse* command_response);

  DBCommandResponse::Status Migrate(
      int32_t version,
      int32_t compatible_version);

  void OnMemoryPressure(
      base::MemoryPressureListener::MemoryPressureLevel memory_pressure_level);

  const base::FilePath db_path_;
  sql::Database db_;
  sql::MetaTable meta_table_;
  bool initialized_;

  std::unique_ptr<base::MemoryPressureListener> memory_pressure_listener_;

  SEQUENCE_CHECKER(sequence_checker_);
};

}  // namespace ledger

#endif  // BAT_LEDGER_LEDGER_DATABASE_H_
