// Base32 encoding implementation
//

#pragma once

std::string base32_encode(const std::string &data);
std::string base32_decode(const std::string &encoded);

// Helper - converts "W2UETLWDY3A4PCONY3GK" to "W2UET-LWDY3-A4PCO-NY3GK"
std::string GroupStringByHyphens(const std::string &s, unsigned int symbols_in_group);
