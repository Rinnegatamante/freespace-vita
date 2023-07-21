/*
 * $Logfile: $
 * $Revision: 307 $
 * $Date: 2010-02-08 09:09:13 +0100 (Mon, 08 Feb 2010) $
 * $Author: taylor $
 *
 * OpenAL based audio streaming
 *
 * $Log$
 * Revision 1.4  2005/10/01 21:53:06  taylor
 * include file cleanup
 * byte-swap streaming PCM to avoid the endless, loud, static
 *
 * Revision 1.3  2005/08/13 16:59:23  taylor
 * type check
 *
 * Revision 1.2  2005/08/12 20:21:06  taylor
 * woorps!
 *
 * Revision 1.1  2005/08/12 08:44:39  taylor
 * import of FS2_Open audio code which is now *nix only, does not include windows or ogg support that FS2_Open has
 *
 * Revision 1.12  2005/06/24 19:36:49  taylor
 * we only want to have m_data_offset be 0 for oggs since the seeking callback will account for the true offset
 * only extern the one int we need for the -nosound speech fix rather than including the entire header
 *
 * Revision 1.11  2005/06/19 02:45:55  taylor
 * OGG streaming fixes to get data reading right and avoid skipping
 * properly handle seeking in OGG streams
 * compiler warning fix in OpenAL builds
 *
 * Revision 1.10  2005/06/01 09:41:14  taylor
 * bit of cleanup for audiostr-openal and fix a Windows-only enum error
 * bunch of OGG related fixes for Linux and Windows (DirectSound and OpenAL), fixes audio related TBP 3.2 crashes
 * gracefully handle OGG logical bitstream changes, shouldn't even load if there is more than 1
 *
 * Revision 1.9  2005/05/28 19:43:28  taylor
 * debug message fixing
 * a little bit of code clarity
 *
 * Revision 1.8  2005/05/24 03:11:38  taylor
 * an extra bounds check in sound.cpp
 * fix audiostr error when filename is !NULL but 0 in len might hit on SDL debug code
 *
 * Revision 1.7  2005/05/15 06:47:57  taylor
 * don't let the ogg callbacks close the file handle on us, let us do it ourselves to keep things straight
 *
 * Revision 1.6  2005/05/13 23:09:28  taylor
 * Ooops!  Added the wrong version of the streaming patch from Jens
 *
 * Revision 1.5  2005/05/12 17:47:57  taylor
 * use vm_malloc(), vm_free(), vm_realloc(), vm_strdup() rather than system named macros
 *   fixes various problems and is past time to make the switch
 * fix a few streaming errors in OpenAL code (Jens Granseuer)
 * temporary change to help deal with missing music in OpenAL Windows builds
 * don't assert when si->data is NULL unless we really need to check (OpenAL only)
 *
 * Revision 1.4  2005/04/05 11:48:22  taylor
 * remove acm-unix.cpp, replaced by acm-openal.cpp since it's properly cross-platform now
 * better error handling for OpenAL functions
 * Windows can now build properly with OpenAL
 * extra check to make sure we don't try and use too many hardware bases sources
 * fix memory error from OpenAL extension list in certain instances
 *
 * Revision 1.3  2005/04/01 07:33:08  taylor
 * fix hanging on exit with OpenAL
 * some better error handling on OpenAL init and make it more Windows friendly too
 * basic 3d sound stuff for OpenAL, not working right yet
 *
 * Revision 1.2  2005/03/27 08:51:24  taylor
 * this is what coding on an empty stomach will get you
 *
 * Revision 1.1  2005/03/27 05:48:58  taylor
 * initial import of OpenAL streaming (many thanks to Pierre Willenbrock for the missing parts)
 *
 *
 * $NoKeywords: $
 */

#ifdef PLAT_UNIX	// to end of file...
#include <stdint.h>
#include "pstypes.h"
#include "audiostr.h"
#include "ds.h"
#include "acm.h"
#include "cfile.h"
#include "sound.h"
#include "timer.h"


#define MAX_STREAM_BUFFERS 4

// status
#define ASF_FREE	0
#define ASF_USED	1

#define MAX_AUDIO_STREAMS	30

// Constants
#ifndef SUCCESS
#define SUCCESS TRUE        // Error returns for all member functions
#define FAILURE FALSE
#endif // SUCCESS

#define BIGBUF_SIZE					180000			// This can be reduced to 88200 once we don't use any stereo
//#define BIGBUF_SIZE					88300			// This can be reduced to 88200 once we don't use any stereo
ubyte *Wavedata_load_buffer = NULL;		// buffer used for cueing audiostreams
ubyte *Wavedata_service_buffer = NULL;	// buffer used for servicing audiostreams

CRITICAL_SECTION Global_service_lock;

typedef BOOL (*TIMERCALLBACK)(ptr_u);

#define COMPRESSED_BUFFER_SIZE	88300
ubyte *Compressed_buffer = NULL;				// Used to load in compressed data during a cueing interval
ubyte *Compressed_service_buffer = NULL;	// Used to read in compressed data during a service interval

#define AS_HIGHEST_MAX	999999999	// max uncompressed filesize supported is 999 meg


int Audiostream_inited = 0;


static int audiostr_read_word(SDL_RWops *rw, WORD *i)
{
	int rc = SDL_RWread( rw, i, 1, sizeof(WORD) );

	if (rc != sizeof(WORD))
		return 0;

	*i = INTEL_SHORT(*i);

	return 1;
}

static int audiostr_read_dword(SDL_RWops *rw, DWORD *i)
{
	int rc = SDL_RWread( rw, i, 1, sizeof(DWORD) );

	if (rc != sizeof(DWORD))
		return 0;

	*i = INTEL_INT(*i);

	return 1;
}

class Timer
{
public:
    void constructor(void);
    void destructor(void);
    BOOL Create (UINT nPeriod, UINT nRes, uintptr_t dwUser,  TIMERCALLBACK pfnCallback);
protected:
	static Uint32 CALLBACK TimeProc(Uint32 interval, void *dwUser);
    TIMERCALLBACK m_pfnCallback;
    uintptr_t m_dwUser;
    UINT m_nPeriod;
    UINT m_nRes;
    SDL_TimerID m_nIDTimer;
};

class WaveFile
{
public:
	void Init(void);
	void Close(void);
	BOOL Open (const char *pszFilename);
	BOOL Cue (void);
	int	Read (ubyte *pbDest, uint cbSize, int service=1);
	uint GetNumBytesRemaining (void) { return (m_nDataSize - m_nBytesPlayed); }
	uint GetUncompressedAvgDataRate (void) { return (m_nUncompressedAvgDataRate); }
	uint GetDataSize (void) { return (m_nDataSize); }
	uint GetNumBytesPlayed (void) { return (m_nBytesPlayed); }
	ubyte GetSilenceData (void);
	WAVEFORMATEX m_wfmt;					// format of wave file used by Direct Sound
	WAVEFORMATEX *m_pwfmt_original;	// foramt of wave file from actual wave source
	uint m_total_uncompressed_bytes_read;
	uint m_max_uncompressed_bytes_to_read;
	uint m_bits_per_sample_uncompressed;

protected:
	uint m_data_offset;						// number of bytes to actual wave data
	int  m_data_bytes_left;
	SDL_RWops *cfp;

	uint m_wave_format;						// format of wave source (ie WAVE_FORMAT_PCM, WAVE_FORMAT_ADPCM)
	uint m_nBlockAlign;						// wave data block alignment spec
	uint m_nUncompressedAvgDataRate;		// average wave data rate
	uint m_nDataSize;							// size of data chunk
	uint m_nBytesPlayed;						// offset into data chunk
	BOOL m_abort_next_read;

	void			*m_hStream;
	int				m_hStream_open;
	WAVEFORMATEX	m_wfxDest;
};

class AudioStream
{
public:
	AudioStream (void);
	~AudioStream (void);
	BOOL Create (const char *pszFilename);
	BOOL Destroy (void);
	void Play (long volume, int looping);
	int Is_Playing(){ return(m_fPlaying); }
	int Is_Paused(){ return(m_bIsPaused); }
	int Is_Past_Limit() { return m_bPastLimit; }
	void Stop (int paused=0);
	void Stop_and_Rewind (void);
	void Fade_and_Destroy (void);
	void Fade_and_Stop(void);
	void	Set_Volume(long vol);
	long	Get_Volume();
	void	Init_Data();
	void	Set_Byte_Cutoff(unsigned int num_bytes_cutoff);
	void  Set_Default_Volume(long converted_volume) { m_lDefaultVolume = converted_volume; }
	long	Get_Default_Volume() { return m_lDefaultVolume; }
	uint Get_Bytes_Committed(void);
	int	Is_looping() { return m_bLooping; }
	int	status;
	int	type;
	ushort m_bits_per_sample_uncompressed;

protected:
	void Cue (void);
	BOOL WriteWaveData (uint cbSize, uint* num_bytes_written,int service=1);
	BOOL WriteSilence (uint cbSize);
	DWORD GetMaxWriteSize (void);
	BOOL ServiceBuffer (void);
	static BOOL TimerCallback (ptr_u dwUser);

	ALuint m_source_id;   // name of openAL source
	ALuint m_buffer_ids[MAX_STREAM_BUFFERS]; //names of buffers
	int m_play_buffer_id;

	Timer m_timer;              // ptr to Timer object
	WaveFile * m_pwavefile;        // ptr to WaveFile object
	BOOL m_fCued;                  // semaphore (stream cued)
	BOOL m_fPlaying;               // semaphore (stream playing)
	long m_lInService;             // reentrancy semaphore
	uint m_cbBufOffset;            // last write position
	uint m_nBufLength;             // length of sound buffer in msec
	uint m_cbBufSize;              // size of sound buffer in bytes
	uint m_nBufService;            // service interval in msec
	uint m_nTimeStarted;           // time (in system time) playback started

	BOOL	m_bLooping;						// whether or not to loop playback
	BOOL	m_bFade;							// fade out music 
	BOOL	m_bDestroy_when_faded;
	long	m_lVolume;						// volume of stream ( 0 -> -10 000 )
	long	m_lCutoffVolume;
	BOOL	m_bIsPaused;					// stream is stopped, but not rewinded
	ushort	m_silence_written;			// number of bytes of silence written to buffer
	ushort	m_bReadingDone;				// no more bytes to be read from disk, still have remaining buffer to play
	DWORD	m_fade_timer_id;				// timestamp so we know when to start fade
	DWORD	m_finished_id;					// timestamp so we know when we've played #bytes required
	BOOL	m_bPastLimit;					// flag to show we've played past the number of bytes requred
	long	m_lDefaultVolume;
	int		h_result;

	CRITICAL_SECTION write_lock;
};


// Timer class implementation
//
////////////////////////////////////////////////////////////

// constructor
void Timer::constructor(void)
{
	m_nIDTimer = NULL;
}


// Destructor
void Timer::destructor(void)
{
	if (m_nIDTimer) {
		SDL_RemoveTimer(m_nIDTimer);
		m_nIDTimer = NULL;
	}
}

// Create
BOOL Timer::Create (UINT nPeriod, UINT nRes, uintptr_t dwUser, TIMERCALLBACK pfnCallback)
{
	BOOL bRtn = SUCCESS;    // assume success

	Assert(pfnCallback);
	Assert(nPeriod > 10);
	Assert(nPeriod >= nRes);

	m_nPeriod = nPeriod;
	m_nRes = nRes;
	m_dwUser = dwUser;
	m_pfnCallback = pfnCallback;

	if ((m_nIDTimer = SDL_AddTimer (m_nPeriod, TimeProc, (void*) this)) == NULL) {
		nprintf(("SOUND", "SOUND ==> Error, unable to create timer\n"));
		bRtn = FAILURE;
	}

	return (bRtn);
}


// Timer proc for multimedia timer callback set with timeSetTime().
//
// Calls procedure specified when Timer object was created. The 
// dwUser parameter contains "this" pointer for associated Timer object.
// 
Uint32 CALLBACK Timer::TimeProc(Uint32 interval, void *dwUser)
{
    // dwUser contains ptr to Timer object
	Timer * ptimer = (Timer *) dwUser;

    // Call user-specified callback and pass back user specified data
    (ptimer->m_pfnCallback) (ptimer->m_dwUser);

    if (ptimer->m_nPeriod) {
		return interval;
    } else {
		SDL_RemoveTimer(ptimer->m_nIDTimer);
		ptimer->m_nIDTimer = NULL;
		return 0;
    }
}


// WaveFile class implementation
//
////////////////////////////////////////////////////////////

// Constructor
void WaveFile::Init(void)
{
	// Init data members
	m_data_offset = 0;
	cfp = NULL;
	m_pwfmt_original = NULL;
	m_nBlockAlign= 0;
	m_nUncompressedAvgDataRate = 0;
	m_nDataSize = 0;
	m_nBytesPlayed = 0;
	m_total_uncompressed_bytes_read = 0;
	m_max_uncompressed_bytes_to_read = AS_HIGHEST_MAX;

	m_hStream_open = 0;
	m_abort_next_read = FALSE;
}

// Destructor
void WaveFile::Close(void)
{
	// Free memory
	if (m_pwfmt_original) {
		free(m_pwfmt_original);
		m_pwfmt_original = NULL;
	}

	if ( m_hStream_open ) {
		ACM_stream_close((void*)m_hStream);
		m_hStream_open = 0;
	}

	// Close file
	if (cfp) {
		SDL_RWclose( cfp );
		cfp = NULL;
	}
}


// Open
BOOL WaveFile::Open (const char *pszFilename)
{
	int done = FALSE;
	WORD cbExtra = 0;
	BOOL fRtn = SUCCESS;    // assume success
	PCMWAVEFORMAT pcmwf;
	char fullpath[_MAX_PATH];

	m_total_uncompressed_bytes_read = 0;
	m_max_uncompressed_bytes_to_read = AS_HIGHEST_MAX;

	int FileSize, FileOffset;

	if ( !cf_find_file_location(pszFilename, CF_TYPE_ANY, fullpath, &FileSize, &FileOffset ))	{
		goto OPEN_ERROR;
	}

	cfp = SDL_RWFromFile(fullpath, "rb");

	if ( cfp == NULL ) {
		goto OPEN_ERROR;
	}

	// Skip the "RIFF" tag and file size (8 bytes)
	// Skip the "WAVE" tag (4 bytes)
	SDL_RWseek( cfp, 12+FileOffset, SEEK_SET );

	// Now read RIFF tags until the end of file
	DWORD tag, size, next_chunk;

	while(done == FALSE)	{
		if ( !audiostr_read_dword(cfp, &tag) )
			break;

		if ( !audiostr_read_dword(cfp, &size) )
			break;

		next_chunk = SDL_RWtell( cfp );
		next_chunk += size;

		switch( tag )	{
		case 0x20746d66:		// The 'fmt ' tag
			audiostr_read_word(cfp, &pcmwf.wf.wFormatTag);
			audiostr_read_word(cfp, &pcmwf.wf.nChannels);
			audiostr_read_dword(cfp, &pcmwf.wf.nSamplesPerSec);
			audiostr_read_dword(cfp, &pcmwf.wf.nAvgBytesPerSec);
			audiostr_read_word(cfp, &pcmwf.wf.nBlockAlign);
			audiostr_read_word(cfp, &pcmwf.wBitsPerSample);

			if ( pcmwf.wf.wFormatTag != WAVE_FORMAT_PCM ) {
				audiostr_read_word(cfp, &cbExtra);
			}

			// Allocate memory for WAVEFORMATEX structure + extra bytes
			if ( (m_pwfmt_original = (WAVEFORMATEX *) malloc ( sizeof(WAVEFORMATEX)+cbExtra )) != NULL ){
				Assert(m_pwfmt_original != NULL);
				// Copy bytes from temporary format structure
				memcpy (m_pwfmt_original, &pcmwf, sizeof(pcmwf));
				m_pwfmt_original->cbSize = cbExtra;

				// Read those extra bytes, append to WAVEFORMATEX structure
				if (cbExtra != 0) {
					SDL_RWread( cfp, ((ubyte *)(m_pwfmt_original) + sizeof(WAVEFORMATEX)), 1, cbExtra );
				}
			}
			else {
				Int3();		// malloc failed
				goto OPEN_ERROR;
			}	
			break;

		case 0x61746164:		// the 'data' tag
			m_nDataSize = size;	// This is size of data chunk.  Compressed if ADPCM.
			m_data_bytes_left = size;
			m_data_offset = SDL_RWtell( cfp );
			done = TRUE;
			break;

		default:	// unknown, skip it
			break;
		}	// end switch

		SDL_RWseek( cfp, next_chunk, SEEK_SET );
	}

	// At this stage, examine source format, and set up WAVEFORATEX structure for DirectSound.
	// Since DirectSound only supports PCM, force this structure to be PCM compliant.  We will
	// need to convert data on the fly later if our souce is not PCM
	switch ( m_pwfmt_original->wFormatTag ) {
		case WAVE_FORMAT_PCM:
			m_wave_format = WAVE_FORMAT_PCM;
			m_wfmt.wBitsPerSample = m_pwfmt_original->wBitsPerSample;
			break;

		case WAVE_FORMAT_ADPCM:
			m_wave_format = WAVE_FORMAT_ADPCM;
			m_wfmt.wBitsPerSample = 16;
			m_bits_per_sample_uncompressed = 16;
			break;

		default:
			nprintf(("SOUND", "SOUND => Not supporting %d format for playing wave files\n", m_pwfmt_original->wFormatTag));
			//Int3();
			goto OPEN_ERROR;
			break;

	} // end switch
            
	// Set up the WAVEFORMATEX structure to have the right PCM characteristics
	m_wfmt.wFormatTag = WAVE_FORMAT_PCM;
	m_wfmt.nChannels = m_pwfmt_original->nChannels;
	m_wfmt.nSamplesPerSec = m_pwfmt_original->nSamplesPerSec;
	m_wfmt.cbSize = 0;
	m_wfmt.nBlockAlign = (ushort)(( m_wfmt.nChannels * m_wfmt.wBitsPerSample ) / 8);
	m_wfmt.nAvgBytesPerSec = m_wfmt.nBlockAlign * m_wfmt.nSamplesPerSec;

	// Init some member data from format chunk
	m_nBlockAlign = m_pwfmt_original->nBlockAlign;
	m_nUncompressedAvgDataRate = m_wfmt.nAvgBytesPerSec;

	// Cue for streaming
	Cue ();

	// Successful open
	goto OPEN_DONE;
    
OPEN_ERROR:
	// Handle all errors here
	nprintf(("SOUND","SOUND ==> Could not open wave file %s for streaming\n",pszFilename));

	fRtn = FAILURE;
	if (cfp != NULL) {
		// Close file
		SDL_RWclose( cfp );
		cfp = NULL;
	}
	if (m_pwfmt_original)
	{
		free(m_pwfmt_original);
		m_pwfmt_original = NULL;
	}

OPEN_DONE:
	return (fRtn);
}

// Cue
//
// Set the file pointer to the start of wave data
//
BOOL WaveFile::Cue (void)
{
	BOOL fRtn = SUCCESS;    // assume success
	int rval = -1;

	m_total_uncompressed_bytes_read = 0;
	m_max_uncompressed_bytes_to_read = AS_HIGHEST_MAX;

	rval = SDL_RWseek( cfp, m_data_offset, SEEK_SET );

	if ( rval == -1 ) {
		fRtn = FAILURE;
	}

	m_data_bytes_left = m_nDataSize;
	m_abort_next_read = FALSE;

	return fRtn;
}


// Read
//
// Returns number of bytes actually read.
// 
//	Returns -1 if there is nothing more to be read.  This function can return 0, since
// sometimes the amount of bytes requested is too small for the ACM decompression to 
// locate a suitable block
int WaveFile::Read(ubyte *pbDest, uint cbSize, int service)
{
	void	*dest_buf=NULL, *uncompressed_wave_data;
	int				rc, uncompressed_bytes_written;
	uint	src_bytes_used, convert_len, num_bytes_desired=0, num_bytes_read;

//	nprintf(("Alan","Reqeusted: %d\n", cbSize));

	if ( service ) {
		uncompressed_wave_data = Wavedata_service_buffer;
	} else {
		uncompressed_wave_data = Wavedata_load_buffer;
	}

	switch ( m_wave_format ) {
		case WAVE_FORMAT_PCM:
			num_bytes_desired = cbSize;
			dest_buf = pbDest;
			break;

		case WAVE_FORMAT_ADPCM:
			if ( !m_hStream_open ) {
				if ( !ACM_stream_open(m_pwfmt_original, &m_wfxDest, (void**)&m_hStream, m_bits_per_sample_uncompressed)  ) {
					m_hStream_open = 1;
				} else {
					Int3();
				}
			}

			num_bytes_desired = cbSize;
	
			if ( service ) {
				dest_buf = Compressed_service_buffer;
			} else {
				dest_buf = Compressed_buffer;
			}

			if ( num_bytes_desired <= 0 ) {
				num_bytes_desired = 0;
//				nprintf(("Alan","No bytes required for ADPCM time interval\n"));
			} else {
				num_bytes_desired = ACM_query_source_size((void*)m_hStream, cbSize);
//				nprintf(("Alan","Num bytes desired: %d\n", num_bytes_desired));
			}
			break;

		default:
			nprintf(("SOUND", "SOUND => Not supporting %d format for playing wave files\n"));
			Int3();
			break;

	} // end switch

	num_bytes_read = 0;
	convert_len = 0;
	src_bytes_used = 0;

	// read data from disk
	if ( m_data_bytes_left <= 0 ) {
		num_bytes_read = 0;
		uncompressed_bytes_written = 0;
		return -1;
	}

	if ( (m_data_bytes_left > 0) && (num_bytes_desired > 0) ) {
		int actual_read = 0;

		if ( num_bytes_desired <= (uint)m_data_bytes_left ) {
			num_bytes_read = num_bytes_desired;
		}
		else {
			num_bytes_read = m_data_bytes_left;
		}

		actual_read = SDL_RWread( cfp, dest_buf, 1, num_bytes_read );

		if ( (actual_read <= 0) || (m_abort_next_read) ) {
			num_bytes_read = 0;
			uncompressed_bytes_written = 0;
			return -1;
		}

		if ( num_bytes_desired >= (uint)m_data_bytes_left ) {
			m_abort_next_read = 1;			
		}

		num_bytes_read = actual_read;
	}

	// convert data if necessary, to PCM
	if ( m_wave_format == WAVE_FORMAT_ADPCM ) {
		if ( num_bytes_read > 0 ) {
			rc = ACM_convert((void*)m_hStream, (ubyte*)dest_buf, num_bytes_read, (ubyte*)uncompressed_wave_data, BIGBUF_SIZE, &convert_len, &src_bytes_used);

			if ( rc == -1 ) {
				goto READ_ERROR;
			}
			if ( convert_len == 0 ) {
				Int3();
			}
		}

		Assert(src_bytes_used <= num_bytes_read);
		if ( src_bytes_used < num_bytes_read ) {
			// seek back file pointer to reposition before unused source data
			SDL_RWseek( cfp, src_bytes_used - num_bytes_read, SEEK_CUR );
		}

		// Adjust number of bytes left
		m_data_bytes_left -= src_bytes_used;
		m_nBytesPlayed += src_bytes_used;
		uncompressed_bytes_written = convert_len;

		// Successful read, keep running total of number of data bytes read
		goto READ_DONE;
	}
	else {
		// Successful read, keep running total of number of data bytes read
		// Adjust number of bytes left
		m_data_bytes_left -= num_bytes_read;
		m_nBytesPlayed += num_bytes_read;
		uncompressed_bytes_written = num_bytes_read;
#if BYTE_ORDER == BIG_ENDIAN
		if ( m_wave_format == WAVE_FORMAT_PCM ) {
			// swap 16-bit sound data
			if (m_wfmt.wBitsPerSample == 16) {
				ushort *swap_tmp;
				
				for (int i=0; i<uncompressed_bytes_written; i=i+2) {
					swap_tmp = (ushort*)((ubyte*)dest_buf + i);
					*swap_tmp = INTEL_SHORT(*swap_tmp);
				}
			}
		}
#endif
		goto READ_DONE;
	}
    
READ_ERROR:
	num_bytes_read = 0;
	uncompressed_bytes_written = 0;

READ_DONE:
	m_total_uncompressed_bytes_read += uncompressed_bytes_written;
//	nprintf(("Alan","Read: %d\n", uncompressed_bytes_written));
	return (uncompressed_bytes_written);
}


// GetSilenceData
//
// Returns 8 bits of data representing silence for the Wave file format.
//
// Since we are dealing only with PCM format, we can fudge a bit and take
// advantage of the fact that for all PCM formats, silence can be represented
// by a single byte, repeated to make up the proper word size. The actual size
// of a word of wave data depends on the format:
//
// PCM Format       Word Size       Silence Data
// 8-bit mono       1 byte          0x80
// 8-bit stereo     2 bytes         0x8080
// 16-bit mono      2 bytes         0x0000
// 16-bit stereo    4 bytes         0x00000000
//
ubyte WaveFile::GetSilenceData (void)
{
	ubyte bSilenceData = 0;

	// Silence data depends on format of Wave file
	if (m_pwfmt_original) {
		if (m_wfmt.wBitsPerSample == 8) {
			// For 8-bit formats (unsigned, 0 to 255)
			// Packed DWORD = 0x80808080;
			bSilenceData = 0x80;
		} else if (m_wfmt.wBitsPerSample == 16) {
			// For 16-bit formats (signed, -32768 to 32767)
			// Packed DWORD = 0x00000000;
			bSilenceData = 0x00;
		} else {
			Int3();
		}
	} else {
		Int3();
	}

	return (bSilenceData);
}

//
// AudioStream class implementation
//
////////////////////////////////////////////////////////////

// The following constants are the defaults for our streaming buffer operation.
const ushort DefBufferLength          = 2000; // default buffer length in msec
const ushort DefBufferServiceInterval = 250;  // default buffer service interval in msec

// Constructor
AudioStream::AudioStream (void)
{
	SDL_LockMutex(write_lock);
}

// Destructor
AudioStream::~AudioStream (void)
{
	SDL_UnlockMutex(write_lock);
}

void AudioStream::Init_Data ()
{
	m_bLooping = 0;
	m_bFade = FALSE;
	m_fade_timer_id = 0;
	m_finished_id = 0;
	m_bPastLimit = FALSE;
	
	m_bDestroy_when_faded = FALSE;
	m_lVolume = 0;
	m_lCutoffVolume = -10000;
	m_bIsPaused = FALSE;
	m_silence_written = 0;
	m_bReadingDone = FALSE;

	m_pwavefile = NULL;
	m_fPlaying = m_fCued = FALSE;
	m_lInService = FALSE;
	m_cbBufOffset = 0;
	m_nBufLength = DefBufferLength;
	m_cbBufSize = 0;
	m_nBufService = DefBufferServiceInterval;
	m_nTimeStarted = 0;

	memset(m_buffer_ids, 0, sizeof(m_buffer_ids));
	m_source_id = 0;
	m_play_buffer_id = 0;
}

// Create
BOOL AudioStream::Create (const char *pszFilename)
{

	BOOL fRtn = SUCCESS;    // assume success

	Assert(pszFilename);

	Init_Data();

	if (pszFilename) {
		// make 100% sure we got a good filename
		if ( !strlen(pszFilename) )
			return FAILURE;

		// Create a new WaveFile object
		m_pwavefile = (WaveFile *)malloc(sizeof(WaveFile));
		Assert(m_pwavefile);

		if (m_pwavefile) {
			// Call constructor
			m_pwavefile->Init();
			// Open given file
			m_pwavefile->m_bits_per_sample_uncompressed = m_bits_per_sample_uncompressed;
			if (m_pwavefile->Open (pszFilename)) {
				// Calculate sound buffer size in bytes
				// Buffer size is average data rate times length of buffer
				// No need for buffer to be larger than wave data though
				m_cbBufSize = (m_nBufLength/1000) * (m_pwavefile->m_wfmt.wBitsPerSample/8) * m_pwavefile->m_wfmt.nChannels * m_pwavefile->m_wfmt.nSamplesPerSec;
				m_cbBufSize /= MAX_STREAM_BUFFERS;
				// if the requested buffer size is too big then cap it
				m_cbBufSize = (m_cbBufSize > BIGBUF_SIZE) ? BIGBUF_SIZE : m_cbBufSize; 

//				nprintf(("SOUND", "SOUND => Stream buffer created using %d bytes\n", m_cbBufSize));

				// Create sound buffer
				OpenAL_ErrorCheck( alGenBuffers(MAX_STREAM_BUFFERS, m_buffer_ids), return FAILURE );
				
				OpenAL_ErrorCheck( alGenSources(1, &m_source_id), return FAILURE );
				
				OpenAL_ErrorPrint( alSourcef(m_source_id, AL_ROLLOFF_FACTOR, 0) );

				OpenAL_ErrorPrint( alSourcei(m_source_id, AL_SOURCE_RELATIVE, AL_TRUE) );

				ALfloat posv[] = { 0, 0, 0 };
				OpenAL_ErrorPrint( alSourcefv(m_source_id, AL_POSITION, posv) );

				OpenAL_ErrorPrint( alSourcef(m_source_id, AL_GAIN, 1) );

				// Cue for playback
				Cue();
				Snd_sram += m_cbBufSize;
			}
			else {
				// Error opening file
				nprintf(("SOUND", "SOUND => Failed to open wave file: %s\n\r", pszFilename));
				m_pwavefile->Close();
				free(m_pwavefile);
				m_pwavefile = NULL;
				fRtn = FAILURE;
			}   
		}
		else {
			// Error, unable to create WaveFile object
			nprintf(("Sound", "SOUND => Failed to create WaveFile object %s\n\r", pszFilename));
			fRtn = FAILURE;
		}
	}
	else {
		// Error, passed invalid parms
		fRtn = FAILURE;
	}

	return (fRtn);
}

// Destroy
BOOL AudioStream::Destroy (void)
{
	BOOL fRtn = SUCCESS;

	SDL_LockMutex(write_lock);

	// Stop playback
	Stop ();

	// Release sound buffer
	OpenAL_ErrorPrint( alDeleteBuffers(MAX_STREAM_BUFFERS, m_buffer_ids) );
	OpenAL_ErrorPrint( alDeleteSources(1, &m_source_id) );
	Snd_sram -= m_cbBufSize;

	// Delete WaveFile object
	if (m_pwavefile) {
		m_pwavefile->Close();
		free(m_pwavefile);
		m_pwavefile = NULL;
	}

	status = ASF_FREE;

	SDL_UnlockMutex(write_lock);

	return fRtn;
}

// WriteWaveData
//
// Writes wave data to sound buffer. This is a helper method used by Create and
// ServiceBuffer; it's not exposed to users of the AudioStream class.
BOOL AudioStream::WriteWaveData (uint size, uint *num_bytes_written, int service)
{
	BOOL fRtn = SUCCESS;
	ubyte *uncompressed_wave_data;

	*num_bytes_written = 0;

	if ( size == 0 || m_bReadingDone ) {
		return fRtn;
	}

	if ( (m_buffer_ids[0] == 0) || !m_pwavefile ) {
		return fRtn;
	}

	if ( service ) {
		SDL_LockMutex(Global_service_lock);
	}
		    
	if ( service ) {
		uncompressed_wave_data = Wavedata_service_buffer;
	} else {
		uncompressed_wave_data = Wavedata_load_buffer;
	}

	int num_bytes_read = 0;

	// Lock the sound buffer
	num_bytes_read = m_pwavefile->Read(uncompressed_wave_data, m_cbBufSize, service);

	if ( num_bytes_read == -1 ) {
		// means nothing left to read!
		num_bytes_read = 0;
		m_bReadingDone = 1;
	}

	if ( num_bytes_read > 0 ) {
	//	nprintf(("SOUND", "SOUND ==> Queueing %d bytes of Data\n", num_bytes_read));

		// Lock the sound buffer
		ALenum format = AL_FORMAT_MONO8;

		if (m_pwavefile->m_wfmt.nChannels == 1) {
			if (m_pwavefile->m_wfmt.wBitsPerSample == 8) 
				format = AL_FORMAT_MONO8;
			else if (m_pwavefile->m_wfmt.wBitsPerSample == 16) 
				format = AL_FORMAT_MONO16;
		} else if (m_pwavefile->m_wfmt.nChannels == 2) {
			if (m_pwavefile->m_wfmt.wBitsPerSample == 8) 
				format = AL_FORMAT_STEREO8;
			else if (m_pwavefile->m_wfmt.wBitsPerSample == 16) 
				format = AL_FORMAT_STEREO16;
		}

		// unqueue and recycle a processed buffer
		ALint p = 0;
		ALuint bid;

		OpenAL_ErrorPrint( alGetSourcei(m_source_id, AL_BUFFERS_PROCESSED, &p) );

		if ( p > 0 ) {
			OpenAL_ErrorPrint( alSourceUnqueueBuffers(m_source_id, 1, &bid) );
		}

		OpenAL_ErrorCheck( alBufferData(m_buffer_ids[m_play_buffer_id], format, uncompressed_wave_data, num_bytes_read, m_pwavefile->m_wfmt.nSamplesPerSec), return FAILURE );

		OpenAL_ErrorCheck( alSourceQueueBuffers(m_source_id, 1, &m_buffer_ids[m_play_buffer_id]), return FAILURE );

		m_play_buffer_id++;

		if (m_play_buffer_id >= MAX_STREAM_BUFFERS)
			m_play_buffer_id = 0;
	}

	if ( service ) {
		SDL_UnlockMutex(Global_service_lock);
	}
    
	return (fRtn);
}


// WriteSilence
//
// Writes silence to sound buffer. This is a helper method used by
// ServiceBuffer; it's not exposed to users of the AudioStream class.
BOOL AudioStream::WriteSilence (uint size)
{
	BOOL fRtn = SUCCESS;

	// not used currently with the OpenAL code

	return (fRtn);
}


// GetMaxWriteSize
//
// Helper function to calculate max size of sound buffer write operation, i.e. how much
// free space there is in buffer.
DWORD AudioStream::GetMaxWriteSize (void)
{
	DWORD dwMaxSize = m_cbBufSize;
	ALint n, q;

	OpenAL_ErrorCheck( alGetSourcei(m_source_id, AL_BUFFERS_PROCESSED, &n), return 0 );

	OpenAL_ErrorCheck( alGetSourcei(m_source_id, AL_BUFFERS_QUEUED, &q), return 0 );

	if (!n && (q >= MAX_STREAM_BUFFERS)) //all buffers queued
		dwMaxSize = 0;

	//	nprintf(("Alan","Max write size: %d\n", dwMaxSize));
	return (dwMaxSize);
}

#define FADE_VOLUME_INTERVAL	 	 					400		// 100 == 1db
#define VOLUME_ATTENUATION_BEFORE_CUTOFF			3000		//  12db 
BOOL AudioStream::ServiceBuffer (void)
{
	long	vol;
	int	fRtn = TRUE;

	if ( status != ASF_USED )
		return FALSE;

	SDL_LockMutex(write_lock);

	// status may have changed, so lets check once again
	if ( status != ASF_USED ){
		SDL_UnlockMutex(write_lock);

		return FALSE;
	}

	if ( m_bFade == TRUE ) {
		if ( m_lCutoffVolume == -10000 ) {
			vol = Get_Volume();
//			nprintf(("Alan","Volume is: %d\n",vol));
			m_lCutoffVolume = max(vol - VOLUME_ATTENUATION_BEFORE_CUTOFF, -10000);
		}

		vol = Get_Volume();
		vol = vol - FADE_VOLUME_INTERVAL;	// decrease by 1db
//		nprintf(("Alan","Volume is now: %d\n",vol));
		Set_Volume(vol);

//		nprintf(("Sound","SOUND => Volume for stream sound is %d\n",vol));
//		nprintf(("Alan","Cuttoff Volume is: %d\n",m_lCutoffVolume));
		if ( vol < m_lCutoffVolume ) {
			m_bFade = 0;
			m_lCutoffVolume = -10000;
			if ( m_bDestroy_when_faded == TRUE ) {
				SDL_UnlockMutex(write_lock);

				Destroy();	
				// Reset reentrancy semaphore

				return FALSE;
			}
			else {
				Stop_and_Rewind();
				// Reset reentrancy semaphore
				SDL_UnlockMutex(write_lock);

				return TRUE;
			}
		}
	}

	// All of sound not played yet, send more data to buffer
	DWORD dwFreeSpace = GetMaxWriteSize ();

	// Determine free space in sound buffer
	if (dwFreeSpace) {

		// Some wave data remains, but not enough to fill free space
		// Send wave data to buffer, fill remainder of free space with silence
		uint num_bytes_written;

		if (WriteWaveData (dwFreeSpace, &num_bytes_written) == SUCCESS) {
//			nprintf(("Alan","Num bytes written: %d\n", num_bytes_written));

			if ( m_pwavefile->m_total_uncompressed_bytes_read >= m_pwavefile->m_max_uncompressed_bytes_to_read ) {
				m_fade_timer_id = timer_get_milliseconds() + 1700;		// start fading 1.7 seconds from now
				m_finished_id = timer_get_milliseconds() + 2000;		// 2 seconds left to play out buffer
				m_pwavefile->m_max_uncompressed_bytes_to_read = AS_HIGHEST_MAX;
			}

			if ( (m_fade_timer_id>0) && ((uint)timer_get_milliseconds() > m_fade_timer_id) ) {
				m_fade_timer_id = 0;
				Fade_and_Stop();
			}

			if ( (m_finished_id>0) && ((uint)timer_get_milliseconds() > m_finished_id) ) {
				m_finished_id = 0;
				m_bPastLimit = TRUE;
			}

			ALint n = 0;
			// get the number of buffers processed to see if we're done
			OpenAL_ErrorCheck( alGetSourcei(m_source_id, AL_BUFFERS_PROCESSED, &n), return FALSE );

			if ( m_bReadingDone && (n == MAX_STREAM_BUFFERS) ) {
				if ( m_bDestroy_when_faded == TRUE ) {
					SDL_UnlockMutex(write_lock);

					Destroy();
					// Reset reentrancy semaphore

					return FALSE;
				}
				// All of sound has played, stop playback or loop again
				if ( m_bLooping && !m_bFade) {
					Play(m_lVolume, m_bLooping);
				}
				else {
					Stop_and_Rewind();
				}
			}
		}
		else {
			// Error writing wave data
			fRtn = FALSE;
			Int3(); 
		}
	}


	SDL_UnlockMutex(write_lock);

	return (fRtn);
}

// Cue
void AudioStream::Cue (void)
{
	uint num_bytes_written;

	if (!m_fCued) {
		m_bFade = FALSE;
		m_fade_timer_id = 0;
		m_finished_id = 0;
		m_bPastLimit = FALSE;
		m_lVolume = 0;
		m_lCutoffVolume = -10000;

		m_bDestroy_when_faded = FALSE;

		// Reset buffer ptr
		m_cbBufOffset = 0;

		// Reset file ptr, etc
		m_pwavefile->Cue ();

		// Unqueue all buffers
		ALint p = 0;
		OpenAL_ErrorPrint( alGetSourcei(m_source_id, AL_BUFFERS_PROCESSED, &p) );
		OpenAL_ErrorPrint( alSourceUnqueueBuffers(m_source_id, p, m_buffer_ids) );

		// Fill buffer with wave data
		WriteWaveData (m_cbBufSize, &num_bytes_written, 0);

		m_fCued = TRUE;
	}
}

// Play
void AudioStream::Play (long volume, int looping)
{
	if (m_buffer_ids[0] != 0) {
		// If playing, stop
		if (m_fPlaying) {
			if ( m_bIsPaused == FALSE)
				Stop_and_Rewind();
		}

		// Cue for playback if necessary
		if (!m_fCued) {
			Cue ();
		}

		if ( looping )
			m_bLooping = 1;
		else
			m_bLooping = 0;

		OpenAL_ErrorPrint( alSourcePlay(m_source_id) );

		m_nTimeStarted = timer_get_milliseconds();
		Set_Volume(volume);

		// Kick off timer to service buffer
		m_timer.constructor();

		m_timer.Create (m_nBufService, m_nBufService, ptr_u (this), TimerCallback);

		// Playback begun, no longer cued
		m_fPlaying = TRUE;
		m_bIsPaused = FALSE;
	}
}

// Timer callback for Timer object created by ::Play method.
BOOL AudioStream::TimerCallback (ptr_u dwUser)
{
    // dwUser contains ptr to AudioStream object
    AudioStream * pas = (AudioStream *) dwUser;

    return (pas->ServiceBuffer ());
}

void AudioStream::Set_Byte_Cutoff(unsigned int byte_cutoff)
{
	if ( m_pwavefile == NULL )
		return;

	m_pwavefile->m_max_uncompressed_bytes_to_read = byte_cutoff;
}

unsigned int AudioStream::Get_Bytes_Committed(void)
{
	if ( m_pwavefile == NULL )
		return 0;

	return m_pwavefile->m_total_uncompressed_bytes_read;
}


// Fade_and_Destroy
void AudioStream::Fade_and_Destroy (void)
{
	m_bFade = TRUE;
	m_bDestroy_when_faded = TRUE;
}

// Fade_and_Destroy
void AudioStream::Fade_and_Stop (void)
{
	m_bFade = TRUE;
	m_bDestroy_when_faded = FALSE;
}


// Stop
void AudioStream::Stop(int paused)
{
	if (m_fPlaying) {
		if (paused) {
			OpenAL_ErrorPrint( alSourcePause(m_source_id) );
		} else {
			OpenAL_ErrorPrint( alSourceStop(m_source_id) );
		}

		m_fPlaying = FALSE;
		m_bIsPaused = paused;

		// Delete Timer object
		m_timer.destructor();
	}
}

// Stop_and_Rewind
void AudioStream::Stop_and_Rewind (void)
{
	if (m_fPlaying) {
		// Stop playback
		OpenAL_ErrorPrint( alSourceStop(m_source_id) );

		// Delete Timer object
		m_timer.destructor();

		m_fPlaying = FALSE;
	}

	m_fCued = FALSE;	// this will cause wave file to start from beginning
	m_bReadingDone = FALSE;
}

// Set_Volume
void AudioStream::Set_Volume(long vol)
{
	if ( vol < -10000 )
		vol = -10000;
	
	if ( vol > 0 )
		vol = 0;

	Assert( vol >= -10000 && vol <= 0 );

	ALfloat alvol = (vol != -10000) ? powf(10.0f, (float)vol / (-600.0f / log10f(.5f))): 0.0f;

	OpenAL_ErrorPrint( alSourcef(m_source_id, AL_GAIN, alvol) );

	m_lVolume = vol;
	if ( h_result != 0 )
		nprintf(("Sound","SOUND => SetVolume() failed with code '%s'\n", get_DSERR_text(h_result) ));
}


// Set_Volume
long AudioStream::Get_Volume()
{
	return m_lVolume;
}



AudioStream Audio_streams[MAX_AUDIO_STREAMS];


void audiostream_init()
{
	int i;

	if ( Audiostream_inited == 1 )
		return;

	if ( !ACM_is_inited() ) {
		return;
	}

	// Allocate memory for the buffer which holds the uncompressed wave data that is streamed from the
	// disk during a load/cue
	if ( Wavedata_load_buffer == NULL ) {
		Wavedata_load_buffer = (ubyte*)malloc(BIGBUF_SIZE);
		Assert(Wavedata_load_buffer != NULL);
	}

	// Allocate memory for the buffer which holds the uncompressed wave data that is streamed from the
	// disk during a service interval
	if ( Wavedata_service_buffer == NULL ) {
		Wavedata_service_buffer = (ubyte*)malloc(BIGBUF_SIZE);
		Assert(Wavedata_service_buffer != NULL);
	}

	// Allocate memory for the buffer which holds the compressed wave data that is read from the hard disk
	if ( Compressed_buffer == NULL ) {
		Compressed_buffer = (ubyte*)malloc(COMPRESSED_BUFFER_SIZE);
		Assert(Compressed_buffer != NULL);
	}

	if ( Compressed_service_buffer == NULL ) {
		Compressed_service_buffer = (ubyte*)malloc(COMPRESSED_BUFFER_SIZE);
		Assert(Compressed_service_buffer != NULL);
	}

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		Audio_streams[i].Init_Data();
		Audio_streams[i].status = ASF_FREE;
		Audio_streams[i].type = ASF_NONE;
	}

	SDL_InitSubSystem(SDL_INIT_TIMER);

	Global_service_lock = SDL_CreateMutex();

	Audiostream_inited = 1;
}

// Close down the audiostream system.  Must call audiostream_init() before any audiostream functions can
// be used.
void audiostream_close()
{
	if ( Audiostream_inited == 0 )
		return;

	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_USED ) {
			Audio_streams[i].status = ASF_FREE;
			Audio_streams[i].Destroy();
		}
	}

	// free global buffers
	if ( Wavedata_load_buffer ) {
		free(Wavedata_load_buffer);
		Wavedata_load_buffer = NULL;
	}

	if ( Wavedata_service_buffer ) {
		free(Wavedata_service_buffer);
		Wavedata_service_buffer = NULL;
	}

	if ( Compressed_buffer ) {
		free(Compressed_buffer);
		Compressed_buffer = NULL;
	}

	if ( Compressed_service_buffer ) {
		free(Compressed_service_buffer);
		Compressed_service_buffer = NULL;
	}

	SDL_DestroyMutex( Global_service_lock );

	Audiostream_inited = 0;

}

// Open a digital sound file for streaming
//
// input:	filename	=>	disk filename of sound file
//				type		=> what type of audio stream do we want to open:
//									ASF_SOUNDFX
//									ASF_EVENTMUSIC
//									ASF_VOICE
//	
// returns:	success => handle to identify streaming sound
//				failure => -1
int audiostream_open( const char *filename, int type )
{
	int i, rc;

	if (!Audiostream_inited || !snd_is_inited())
		return -1;

	for ( i=0; i<MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE ) {
			Audio_streams[i].status = ASF_USED;
			Audio_streams[i].type = type;
			break;
		}
	}

	if ( i == MAX_AUDIO_STREAMS ) {
		nprintf(("Sound", "SOUND => No more audio streams available!\n"));
		return -1;
	}

	switch(type) {
		case ASF_VOICE:
		case ASF_SOUNDFX:
			Audio_streams[i].m_bits_per_sample_uncompressed = 8;
		case ASF_EVENTMUSIC:
			Audio_streams[i].m_bits_per_sample_uncompressed = 16;
			break;
		default:
			Int3();
			return -1;
	}

	rc = Audio_streams[i].Create(filename);

	if ( rc == 0 ) {
		Audio_streams[i].status = ASF_FREE;
		return -1;
	} else {
		return i;
	}
}

void audiostream_close_file(int i, int fade)
{
	if (!Audiostream_inited)
		return;

	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );

	if ( Audio_streams[i].status == ASF_USED ) {
		if ( fade == TRUE ) {
			Audio_streams[i].Fade_and_Destroy();
		} else {
			Audio_streams[i].Destroy();
		}
	}

}

void audiostream_close_all(int fade)
{
	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE )
			continue;

		audiostream_close_file(i, fade);
	}
}

void audiostream_play(int i, float volume, int looping)
{
	if (!Audiostream_inited)
		return;

	if ( i == -1 )
		return;

	Assert(looping >= 0);
	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );

	// convert from 0->1 to -10000->0 for volume
	int converted_volume;
	if ( volume == -1 ) {
		converted_volume = Audio_streams[i].Get_Default_Volume();
	}
	else {
		Assert(volume >= 0.0f && volume <= 1.0f );
		converted_volume = ds_convert_volume(volume);
	}

	Assert( Audio_streams[i].status == ASF_USED );
	Audio_streams[i].Set_Default_Volume(converted_volume);
	Audio_streams[i].Play(converted_volume, looping);
}

// use as buffer service function
int audiostream_is_playing(int i)
{
	if ( i == -1 )
		return 0;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	if ( Audio_streams[i].status != ASF_USED )
		return 0;

	return Audio_streams[i].Is_Playing();
}

void audiostream_stop(int i, int rewind, int paused)
{
	if (!Audiostream_inited)
		return;

	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	Assert( Audio_streams[i].status == ASF_USED );

	if ( rewind )
		Audio_streams[i].Stop_and_Rewind();
	else
		Audio_streams[i].Stop(paused);
}

void audiostream_set_volume_all(float volume, int type)
{
	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE )
			continue;

		if ( Audio_streams[i].type == type ) {
			int converted_volume;
			converted_volume = ds_convert_volume(volume);
			Audio_streams[i].Set_Volume(converted_volume);
		}
	}
}

void audiostream_set_volume(int i, float volume)
{
	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	Assert( volume >= 0 && volume <= 1);

	if ( Audio_streams[i].status == ASF_FREE )
		return;

	int converted_volume;
	converted_volume = ds_convert_volume(volume);
	Audio_streams[i].Set_Volume(converted_volume);
}

int audiostream_is_paused(int i)
{
	if ( i == -1 )
		return 0;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	if ( Audio_streams[i].status == ASF_FREE )
		return -1;

	BOOL is_paused;
	is_paused = Audio_streams[i].Is_Paused();

	return is_paused;
}

void audiostream_set_byte_cutoff(int i, unsigned int cutoff)
{
	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	Assert( cutoff > 0 );

	if ( Audio_streams[i].status == ASF_FREE )
		return;

	Audio_streams[i].Set_Byte_Cutoff(cutoff);
}

uint audiostream_get_bytes_committed(int i)
{
	if ( i == -1 )
		return 0;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );

	if ( Audio_streams[i].status == ASF_FREE )
		return 0;

	uint num_bytes_committed;
	num_bytes_committed = Audio_streams[i].Get_Bytes_Committed();
	
	return num_bytes_committed;
}

int audiostream_done_reading(int i)
{
	if ( i == -1 )
		return 0;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );

	if ( Audio_streams[i].status == ASF_FREE )
		return 0;

	int done_reading;
	done_reading = Audio_streams[i].Is_Past_Limit();
	
	return done_reading;
}

int audiostream_is_inited()
{
	return Audiostream_inited;
}

void audiostream_pause(int i)
{
	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	if ( Audio_streams[i].status == ASF_FREE )
		return;

	if ( audiostream_is_playing(i) == TRUE ) {
		audiostream_stop(i, 0, 1);
	}
}

void audiostream_pause_all()
{
	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE )
			continue;

		audiostream_pause(i);
	}
}

void audiostream_unpause(int i)
{
	int is_looping;

	if ( i == -1 )
		return;

	Assert( i >= 0 && i < MAX_AUDIO_STREAMS );
	if ( Audio_streams[i].status == ASF_FREE )
		return;

	if ( audiostream_is_paused(i) == TRUE ) {
		is_looping = Audio_streams[i].Is_looping();
		audiostream_play(i, -1.0f, is_looping);
	}
}

void audiostream_unpause_all()
{
	int i;

	for ( i = 0; i < MAX_AUDIO_STREAMS; i++ ) {
		if ( Audio_streams[i].status == ASF_FREE )
			continue;

		audiostream_unpause(i);
	}
}

void audiostream_set_sample_cutoff(int i, uint cutoff)
{
//	STUB_FUNCTION;
}

uint audiostream_get_samples_committed(int i)
{
//	STUB_FUNCTION;

	return 0;
}

#endif	// PLAT_UNIX
