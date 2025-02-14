/*
 * Copyright (c) 2022, Linus Groh <linusg@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/Fetch/Infrastructure/HTTP/Responses.h>

namespace Web::Fetch {

// https://fetch.spec.whatwg.org/#ref-for-concept-network-error%E2%91%A3
// A network error is a response whose status is always 0, status message is always
// the empty byte sequence, header list is always empty, and body is always null.

Response Response::aborted_network_error()
{
    auto response = network_error();
    response.set_aborted(true);
    return response;
}

Response Response::network_error()
{
    Response response;
    response.set_status(0);
    response.set_type(Type::Error);
    VERIFY(!response.body().has_value());
    return response;
}

// https://fetch.spec.whatwg.org/#concept-aborted-network-error
bool Response::is_aborted_network_error() const
{
    // A response whose type is "error" and aborted flag is set is known as an aborted network error.
    return m_type == Type::Error && m_aborted;
}

// https://fetch.spec.whatwg.org/#concept-network-error
bool Response::is_network_error() const
{
    // A response whose type is "error" is known as a network error.
    return m_type == Type::Error;
}

// https://fetch.spec.whatwg.org/#concept-response-url
Optional<AK::URL const&> Response::url() const
{
    // A response has an associated URL. It is a pointer to the last URL in response’s URL list and null if response’s URL list is empty.
    if (m_url_list.is_empty())
        return {};
    return m_url_list.last();
}

// https://fetch.spec.whatwg.org/#concept-response-location-url
ErrorOr<Optional<AK::URL>> Response::location_url(Optional<String> const& request_fragment) const
{
    // The location URL of a response response, given null or an ASCII string requestFragment, is the value returned by the following steps. They return null, failure, or a URL.

    // 1. If response’s status is not a redirect status, then return null.
    if (!is_redirect_status(m_status))
        return Optional<AK::URL> {};

    // FIXME: 2. Let location be the result of extracting header list values given `Location` and response’s header list.
    auto location_value = ByteBuffer {};

    // 3. If location is a header value, then set location to the result of parsing location with response’s URL.
    auto location = AK::URL { StringView { location_value } };
    if (!location.is_valid())
        return Error::from_string_view("Invalid 'Location' header URL"sv);

    // 4. If location is a URL whose fragment is null, then set location’s fragment to requestFragment.
    if (location.fragment().is_null())
        location.set_fragment(request_fragment.value_or({}));

    // 5. Return location.
    return location;
}

FilteredResponse::FilteredResponse(Response& internal_response)
    : m_internal_response(internal_response)
{
}

FilteredResponse::~FilteredResponse()
{
}

ErrorOr<BasicFilteredResponse> BasicFilteredResponse::create(Response& internal_response)
{
    // A basic filtered response is a filtered response whose type is "basic" and header list excludes
    // any headers in internal response’s header list whose name is a forbidden response-header name.
    HeaderList header_list;
    for (auto const& header : internal_response.header_list()) {
        if (!is_forbidden_response_header_name(header.name))
            TRY(header_list.append(header));
    }

    return BasicFilteredResponse(internal_response, move(header_list));
}

BasicFilteredResponse::BasicFilteredResponse(Response& internal_response, HeaderList header_list)
    : FilteredResponse(internal_response)
    , m_header_list(move(header_list))
{
}

ErrorOr<CORSFilteredResponse> CORSFilteredResponse::create(Response& internal_response)
{
    // A CORS filtered response is a filtered response whose type is "cors" and header list excludes
    // any headers in internal response’s header list whose name is not a CORS-safelisted response-header
    // name, given internal response’s CORS-exposed header-name list.
    Vector<ReadonlyBytes> cors_exposed_header_name_list;
    for (auto const& header_name : internal_response.cors_exposed_header_name_list())
        cors_exposed_header_name_list.append(header_name.span());

    HeaderList header_list;
    for (auto const& header : internal_response.header_list()) {
        if (is_cors_safelisted_response_header_name(header.name, cors_exposed_header_name_list))
            TRY(header_list.append(header));
    }

    return CORSFilteredResponse(internal_response, move(header_list));
}

CORSFilteredResponse::CORSFilteredResponse(Response& internal_response, HeaderList header_list)
    : FilteredResponse(internal_response)
    , m_header_list(move(header_list))
{
}

OpaqueFilteredResponse OpaqueFilteredResponse::create(Response& internal_response)
{
    // An opaque-redirect filtered response is a filtered response whose type is "opaqueredirect",
    // status is 0, status message is the empty byte sequence, header list is empty, and body is null.
    return OpaqueFilteredResponse(internal_response);
}

OpaqueFilteredResponse::OpaqueFilteredResponse(Response& internal_response)
    : FilteredResponse(internal_response)
{
}

OpaqueRedirectFilteredResponse OpaqueRedirectFilteredResponse::create(Response& internal_response)
{
    return OpaqueRedirectFilteredResponse(internal_response);
}

OpaqueRedirectFilteredResponse::OpaqueRedirectFilteredResponse(Response& internal_response)
    : FilteredResponse(internal_response)
{
}

}
