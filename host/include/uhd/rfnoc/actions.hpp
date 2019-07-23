//
// Copyright 2019 Ettus Research, a National Instruments Brand
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#ifndef INCLUDED_LIBUHD_RFNOC_ACTIONS_HPP
#define INCLUDED_LIBUHD_RFNOC_ACTIONS_HPP

#include <uhd/config.hpp>
#include <uhd/types/device_addr.hpp>
#include <uhd/types/metadata.hpp>
#include <uhd/types/stream_cmd.hpp>
#include <memory>
#include <string>
#include <vector>

namespace uhd { namespace rfnoc {

/*! Container for an action
 *
 * In the RFNoC context, an action is comparable to a command. Nodes in the
 * graph can send each other actions. action_info is the payload of such an
 * action message. Nodes pass shared pointers to action_info objects between
 * each other to avoid costly copies of large action_info objects.
 */
struct UHD_API action_info
{
public:
    virtual ~action_info() {}

    using sptr = std::shared_ptr<action_info>;
    //! A unique counter for this action
    const size_t id;
    //! A string identifier for this action
    std::string key;
    //! An arbitrary payload. It is up to consumers and producers to
    // (de-)serialize it.
    std::vector<uint8_t> payload;
    //! A dictionary of key-value pairs. May be used as desired.
    uhd::device_addr_t args;

    //! Factory function
    static sptr make(const std::string& key = "",
        const uhd::device_addr_t& args      = uhd::device_addr_t(""));

protected:
    action_info(
        const std::string& key, const uhd::device_addr_t& args = uhd::device_addr_t(""));
};

struct UHD_API stream_cmd_action_info : public action_info
{
public:
    using sptr = std::shared_ptr<stream_cmd_action_info>;

    uhd::stream_cmd_t stream_cmd;

    //! Factory function
    static sptr make(const uhd::stream_cmd_t::stream_mode_t stream_mode);

private:
    stream_cmd_action_info(const uhd::stream_cmd_t::stream_mode_t stream_mode);
};

struct UHD_API rx_event_action_info : public action_info
{
public:
    using sptr = std::shared_ptr<rx_event_action_info>;

    //! The error code that describes the event
    uhd::rx_metadata_t::error_code_t error_code = uhd::rx_metadata_t::ERROR_CODE_NONE;

    //! Factory function
    static sptr make();

private:
    rx_event_action_info();
};

}} /* namespace uhd::rfnoc */

#endif /* INCLUDED_LIBUHD_RFNOC_ACTIONS_HPP */

