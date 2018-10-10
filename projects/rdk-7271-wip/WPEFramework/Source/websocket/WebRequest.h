#ifndef __WEBREQUEST_H
#define __WEBREQUEST_H

#include "Module.h"

namespace WPEFramework {
namespace Web {
    static const uint8 MajorVersion = 1;
    static const uint8 MinorVersion = 1;

    enum MarshalType {
        MARSHAL_RAW,
        MARSHAL_UPPERCASE
    };

    enum MIMETypes {
        MIME_BINARY,
        MIME_TEXT,
        MIME_HTML,
        MIME_JSON,
        MIME_XML,
        MIME_JS,
        MIME_CSS,
        MIME_IMAGE_TIFF,
        MIME_IMAGE_VND,
        MIME_IMAGE_X_ICON,
        MIME_IMAGE_X_JNG,
        MIME_IMAGE_X_BMP,
        MIME_IMAGE_X_MS_BMP,
        MIME_IMAGE_SVG_XML,
        MIME_IMAGE_WEBP,
        MIME_IMAGE_GIF,
        MIME_IMAGE_JPG,
        MIME_IMAGE_PNG,
        MIME_FONT_TTF,
        MIME_FONT_OPENTYPE,
        MIME_TEXT_XML,
        MIME_APPLICATION_FONT_WOFF,
        MIME_APPLICATION_JAVA_ARCHIVE,
        MIME_APPLICATION_JAVASCRIPT,
        MIME_APPLICATION_ATOM_XML,
        MIME_APPLICATION_RSS_XML,
        MIME_UNKNOWN
    };

    bool EXTERNAL MIMETypeForFile(const string path, string& fileToService, MIMETypes& mimeType);

    enum EncodingTypes {
        ENCODING_GZIP,
        ENCODING_UNKNOWN
    };

    enum TransferTypes {
        TRANSFER_CHUNKED,
        TRANSFER_UNKNOWN
    };

    enum CharacterTypes {
        CHARACTER_UTF8,
        CHARACTER_UNKNOWN
    };

    struct EXTERNAL IBody {
        virtual ~IBody(){};

        // The Serialize/Deserialize methods mark the start of an upcoming serialization/deserialization
        // of the object. These methods allow for preparation of content to be Serialised or Deserialized.
        // The return value is of the Serialization inidcateds the number of bytes required for the body
        // The reurn value of the Deserialize indicate the number of byes that could by loaded as max.
        virtual uint32 Serialize() const = 0;
        virtual uint32 Deserialize() = 0;

        // The End method indicates a completion of the Serialization or Deserialization.
        virtual void End() const = 0;

        // The Serialize and Deserialize methods allow the content to be serialized/deserialized.
        virtual void Serialize(uint8[] /* stream*/, const uint16 /* maxLength */) const = 0;
        virtual void Deserialize(const uint8[] /* stream*/, const uint16 /* maxLength */) = 0;
    };

    class EXTERNAL Signature {
    public:
        Signature()
            : _type(Crypto::HASH_MD5)
        {
        }
        Signature(const Crypto::EnumHashType& type, const uint8 hash[])
            : _type(type)
        {
            ASSERT(_type <= sizeof(_hashValue));
            ::memcpy(_hashValue, hash, _type);
        }
        Signature(const Signature& copy)
            : _type(copy._type)
        {
            ASSERT(_type <= sizeof(_hashValue));
            ::memcpy(_hashValue, copy._hashValue, _type);
        }
        ~Signature()
        {
        }

        Signature& operator=(const Signature& RHS)
        {
            _type = RHS._type;
            ::memcpy(_hashValue, RHS._hashValue, _type);

            return (*this);
        }

    public:
        bool Equal(const Crypto::EnumHashType& type, const uint8 value[]) const
        {
            return ((_type == type) && (::memcmp(_hashValue, value, _type) == 0));
        }
        inline bool operator==(const Signature& RHS) const
        {
            return ((RHS._type == _type) && (memcmp(RHS._hashValue, _hashValue, _type) == 0));
        }
        inline bool operator!=(const Signature& RHS) const
        {
            return (!(operator==(RHS)));
        }
        Crypto::EnumHashType Type() const
        {
            return (_type);
        }
        const uint8* Data() const
        {
            return (_hashValue);
        }

    private:
        uint8 _hashValue[64];
        Crypto::EnumHashType _type;
    };

    class EXTERNAL Request {
    public:
        typedef Request BaseElement;

        enum keywords {
            HOST,
            UPGRADE,
            CONNECTION,
            ORIGIN,
            ACCEPT,
            ACCEPT_ENCODING,
            USERAGENT,
            CONTENT_TYPE,
            CONTENT_LENGTH,
            CONTENT_SIGNATURE,
            CONTENT_ENCODING,
            TRANSFER_ENCODING,
            ENCODING,
            LANGUAGE,
            ACCESS_CONTROL_REQUEST_METHOD,
            ACCESS_CONTROL_REQUEST_HEADERS,
            WEBSOCKET_KEY,
            WEBSOCKET_PROTOCOL,
            WEBSOCKET_VERSION,
            WEBSOCKET_EXTENSIONS,
            MAN,
            M_X,
            S_T
        };

        enum type {
            HTTP_NONE = 0x0000,
            HTTP_GET = 0x0001,
            HTTP_HEAD = 0x0002,
            HTTP_POST = 0x0004,
            HTTP_PUT = 0x0008,
            HTTP_DELETE = 0x0010,
            HTTP_OPTIONS = 0x0020,
            HTTP_TRACE = 0x0040,
            HTTP_CONNECT = 0x0080,
            HTTP_PATCH = 0x0100,
            HTTP_MSEARCH = 0x0200,
            HTTP_NOTIFY = 0x0400,
            HTTP_UNKNOWN = 0x8000
        };

        enum upgrade {
            UPGRADE_UNKNOWN,
            UPGRADE_WEBSOCKET
        };

        enum connection {
            CONNECTION_UNKNOWN,
            CONNECTION_UPGRADE,
            CONNECTION_KEEPALIVE,
            CONNECTION_CLOSE
        };

        class EXTERNAL Serializer {
        private:
            enum enumState {
                VERB = 1,
                URL = 2,
                QUERY = 3,
                FRAGMENT = 4,
                VERSION = 5,
                PAIR_KEY = 6,
                PAIR_VALUE = 7,
                BODY = 8,
                REPORT = 9
            };
            const static uint16 EOL_MARKER = 0x8000;

            Serializer(const Serializer&) = delete;
            Serializer& operator=(const Serializer&) = delete;

        public:
            Serializer()
                : _state(VERB)
                , _offset(0)
                , _keyIndex(0)
                , _value()
                , _buffer(NULL)
                , _lock()
                , _current()
            {
            }
            ~Serializer()
            {
            }

        public:
            virtual void Serialized(const Web::Request& element) = 0;

            void Flush()
            {
                _lock.Lock();
                _state = VERSION;
                Web::Request* backup = _current;
                _current = NULL;
                if (backup != NULL) {
                    Serialized(*backup);
                }
                _lock.Unlock();
            }

            void Submit(const Web::Request& element)
            {
                _lock.Lock();

                ASSERT(_current == NULL);

                _current = const_cast<Request*>(&element);

                _lock.Unlock();
            }

            uint16 Serialize(uint8 stream[], const uint16 maxLength);

        private:
            uint16 _state;
            uint16 _offset;
            uint8 _keyIndex;
            string _value;
            uint32 _bodyLength;
            const TCHAR* _buffer;
            Core::CriticalSection _lock;
            Request* _current;
        };
        class EXTERNAL Deserializer {
        private:
            enum enumState {
                VERB,
                URL,
                VERSION,
                PAIR_KEY,
                PAIR_VALUE,
                CHUNK_INIT,
                CHUNK_END,
                BODY_END
            };
            typedef Core::ParserType<Core::TerminatorCarriageReturnLineFeed, Deserializer> Parser;

            Deserializer(const Deserializer&) = delete;
            Deserializer& operator=(const Deserializer&) = delete;

        public:
#ifdef __WIN32__
#pragma warning(disable : 4355)
#endif
            Deserializer()
                : _lock()
                , _current()
                , _state(VERB)
                , _parser(*this)
            {
            }
#ifdef __WIN32__
#pragma warning(default : 4355)
#endif
            ~Deserializer()
            {
            }

        public:
            inline void Flush()
            {
                _lock.Lock();

                _state = VERB;
                _parser.Reset();

                if (_current != NULL) {
                    Web::Request* element = _current;
                    _current = NULL;

                    Deserialized(*element);
                }

                _lock.Unlock();
            }
            inline uint16 Deserialize(const uint8 stream[], const uint16 maxLength)
            {
                _lock.Lock();

                uint16 usedSize = _parser.Deserialize(stream, maxLength);

                _lock.Unlock();

                return (usedSize);
            }

            // The whole request object is deserialised..
            virtual void Deserialized(Web::Request& element) = 0;

            // We need a request object to be able to fill it with info
            virtual Web::Request* Element() = 0;

            // We reached the body, link a proper body to the response..
            virtual bool LinkBody(Web::Request& response) = 0;

        private:
            friend class Core::ParserType<Core::TerminatorCarriageReturnLineFeed, Deserializer>;

            void Parse(const uint8 stream[], const uint16 maxLength);
            void Parse(const string& buffer);
            void EndOfLine();
            void EndOfPassThrough();

        private:
            Core::CriticalSection _lock;
            Web::Request* _current;
            enumState _state;
            Web::Request::keywords _keyWord;
            Parser _parser;
            z_stream _zlib;
            uint32 _zlibResult;
        };

    private:
        Request(const Request&) = delete;
        Request& operator=(const Request&) = delete;

    public:
        Request()
            : _body()
        {
            Clear();
        }
        ~Request()
        {
        }

    public:
		static const TCHAR* GET;
		static const TCHAR* HEAD;
		static const TCHAR* POST;
		static const TCHAR* PUT;
		static const TCHAR* DELETE;
		static const TCHAR* OPTIONS;
		static const TCHAR* TRACE;
		static const TCHAR* CONNECT;
		static const TCHAR* PATCH;
		static const TCHAR* MSEARCH;
		static const TCHAR* NOTIFY;

		static const TCHAR* ToString(const type value);
        static void ToString(const Request& realObject, string& text)
        {
            uint16 fillCount = 0;
            bool ready = false;
            class SerializerImpl : public Serializer {
            public:
                SerializerImpl(bool& readyFlag)
                    : Serializer()
                    , _ready(readyFlag)
                {
                }
                virtual ~SerializerImpl()
                {
                }

            public:
                virtual void Serialized(const Request& /* element */)
                {
                    _ready = true;
                }

            private:
                bool& _ready;

            } serializer(ready);

            // Request an object to e serialized..
            serializer.Submit(realObject);

            // Serialize object
            while (ready == false) {
                uint8 buffer[1024];
                uint16 loaded = serializer.Serialize(buffer, sizeof(buffer));

                ASSERT(loaded <= sizeof(buffer));

                fillCount += loaded;

                text += string(reinterpret_cast<char*>(&buffer[0]), loaded);
            }
        }
        inline void ToString(string& text) const
        {
            Request::ToString(*this, text);
        }
		static void FromString(Request& realObject, const string& text)
		{
			class DeserializerImpl : public Deserializer {
			public:
				DeserializerImpl(Web::Request& destination)
					: Deserializer()
					, _destination(destination)
				{
				}
				virtual ~DeserializerImpl()
				{
				}

			public:
				// The whole request object is deserialised..
				virtual void Deserialized(Web::Request& element VARIABLE_IS_NOT_USED) {
				}

				// We need a request object to be able to fill it with info
				virtual Web::Request* Element() {
					return (&_destination);
				}

				// We reached the body, link a proper body to the response..
				virtual bool LinkBody(Web::Request& request) {
					return (request.HasBody());
				}

			private:
				Web::Request& _destination;

			} deserializer(realObject);

			// Request an object to e serialized..
			deserializer.Deserialize(reinterpret_cast<const uint8*>(text.c_str()), static_cast<uint16>(text.length()));
		}
		inline void FromString(const string& text)
		{
			Request::FromString(*this, text);
		}
		inline bool IsValid() const
        {
            return (Verb != HTTP_UNKNOWN);
        }
        void Clear()
        {
            _marshalMode = MARSHAL_RAW;
            Verb = HTTP_UNKNOWN;
            Path.clear();
            Query.Clear();
            Fragment.Clear();
            MajorVersion = Web::MajorVersion;
            MinorVersion = Web::MinorVersion;
            Origin.Clear();
            Upgrade.Clear();
            Host.Clear();
            Connection.Clear();
            Accept.Clear();
            AcceptEncoding.Clear();
            UserAgent.Clear();
            Encoding.Clear();
            Language.Clear();
            ContentType.Clear();
            ContentLength.Clear();
            ContentEncoding.Clear();
            ContentCharacterSet.Clear();
            AccessControlHeaders.Clear();
            AccessControlMethod.Clear();
            WebSocketKey.Clear();
            WebSocketProtocol.Clear();
            WebSocketVersion.Clear();
            WebSocketExtensions.Clear();
            ContentSignature.Clear();
            TransferEncoding.Clear();
            Man.Clear();
            MX.Clear();
            ST.Clear();

            if (_body.IsValid() == true) {
                _body.Release();
            }
        }

        type Verb;
        string Path;
        Core::OptionalType<string> Query;
        Core::OptionalType<string> Fragment;
        uint8 MajorVersion;
        uint8 MinorVersion;
        Core::OptionalType<string> Origin;
        Core::OptionalType<upgrade> Upgrade;
        Core::OptionalType<MIMETypes> ContentType;
        Core::OptionalType<CharacterTypes> ContentCharacterSet;
        Core::OptionalType<uint32> ContentLength;
        Core::OptionalType<Signature> ContentSignature;
        Core::OptionalType<EncodingTypes> ContentEncoding;
        Core::OptionalType<TransferTypes> TransferEncoding;
        Core::OptionalType<string> Host;
        Core::OptionalType<connection> Connection;
        Core::OptionalType<string> Accept;
        Core::OptionalType<EncodingTypes> AcceptEncoding;
        Core::OptionalType<string> UserAgent;
        Core::OptionalType<string> Encoding;
        Core::OptionalType<string> Language;
        Core::OptionalType<string> AccessControlHeaders;
        Core::OptionalType<uint16> AccessControlMethod;
        Core::OptionalType<string> WebSocketKey;
        Core::OptionalType<string> WebSocketProtocol;
        Core::OptionalType<uint32> WebSocketVersion;
        Core::OptionalType<string> WebSocketExtensions;
        Core::OptionalType<string> Man;
        Core::OptionalType<string> ST;
        Core::OptionalType<uint32> MX;

        inline bool HasBody() const
        {
            return (_body.IsValid());
        }
        template <typename BODYTYPE>
        inline void Body(const Core::ProxyType<BODYTYPE>& body)
        {
            _body = Core::proxy_cast<IBody>(body);
        }
        template <typename BODYTYPE>
        inline Core::ProxyType<BODYTYPE> Body()
        {
            ASSERT(HasBody() == true);

            return (Core::proxy_cast<BODYTYPE>(_body));
        }
        template <typename BODYTYPE>
        inline Core::ProxyType<const BODYTYPE> Body() const
        {
            ASSERT(HasBody() == true);

            return (Core::proxy_cast<const BODYTYPE>(_body));
        }
        inline void Mode(const MarshalType mode)
        {
            _marshalMode = mode;
        }
        inline MarshalType Mode() const
        {
            return (_marshalMode);
        }

    private:
        Core::ProxyType<IBody> _body;
        MarshalType _marshalMode;
    };
}
}

#endif // __WEBREQUEST_H