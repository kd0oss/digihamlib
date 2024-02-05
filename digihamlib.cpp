#include "digihamlib.h"
#ifdef Q_OS_ANDROID
#include <QCoreApplication>
#include <QJniObject>
#endif
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <cstring>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <QDebug>


Digihamlib::Digihamlib(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<Mode::MODEINFO>("Mode::MODEINFO");
    connect_status = Mode::DISCONNECTED;
    m_mdirect = false;
}

Digihamlib::~Digihamlib()
{

}

void Digihamlib::updatelog(QString info)
{
    emit update_log(info);
}

void Digihamlib::update(Mode::MODEINFO info)
{
    //   Mode::MODEINFO info = m_mode->m_modeinfo;
    if ((connect_status == Mode::CONNECTING) && (info.status == Mode::DISCONNECTED))
    {
        process_connect();
        return;
    }

    if ((connect_status == Mode::CONNECTING) && ( info.status == Mode::CONNECTED_RW))
    {
        connect_status = Mode::CONNECTED_RW;
        //    emit connect_status_changed(2);
        emit swtx_state(!m_mode->get_hwtx());
        emit swrx_state(!m_mode->get_hwrx());
        emit rptr2_changed(m_refname + " " + m_module);
        if(m_mycall.isEmpty()) set_mycall(m_callsign);
        if(m_urcall.isEmpty()) set_urcall("CQCQCQ");
        if(m_rptr1.isEmpty()) set_rptr1(m_callsign + " " + m_module);
        emit update_log("Connected to " + m_protocol + " " + m_refname + " " + m_host + ":" + QString::number(m_port));

        if(info.sw_vocoder_loaded){
            emit update_log("Vocoder plugin loaded");
        }
        else{
            emit update_log("Vocoder plugin not loaded");
            //         emit open_vocoder_dialog();
        }
    }

    m_netstatustxt = "Ping: " + QString::number(info.count);
    m_ambestatustxt = "AMBE: " + (info.ambeprodid.isEmpty() ? "No device" : info.ambeprodid);
    m_mmdvmstatustxt = "MMDVM: ";

    if(info.mmdvm.isEmpty()){
        m_mmdvmstatustxt += "No device";
    }

    QStringList verlist = info.ambeverstr.split('.');
    if(verlist.size() > 7){
        m_ambestatustxt += " " + verlist.at(0) + " " + verlist.at(5) + " " + verlist.at(6);
    }

    verlist = info.mmdvm.split(' ');
    if(verlist.size() > 3){
        m_mmdvmstatustxt += verlist.at(0) + " " + verlist.at(1);
    }

    if(info.stream_state == Mode::STREAM_IDLE){
        m_data1.clear();
        m_data2.clear();
        m_data3.clear();
        m_data4.clear();
        m_data5.clear();
        m_data6.clear();
    }
    else if (m_protocol == "REF" || m_protocol == "XRF" || m_protocol == "DCS"){
        m_data1 = info.src;
        m_data2 = info.dst;
        m_data3 = info.gw;
        m_data4 = info.gw2;
        m_data5 = QString::number(info.streamid, 16) + " " + QString("%1").arg(info.frame_number, 2, 16, QChar('0'));
        m_data6 = info.usertxt;
    }
    else if (m_protocol == "YSF" || m_protocol == "FCS"){
        m_data1 = info.gw;
        m_data2 = info.src;
        m_data3 = info.dst;

        if(info.type == 0){
            m_data4 = "V/D mode 1";
        }
        else if(info.type == 1){
            m_data4 = "Data Full Rate";
        }
        else if(info.type == 2){
            m_data4 = "V/D mode 2";
        }
        else if(info.type == 3){
            m_data4 = "Voice Full Rate";
        }
        else{
            m_data4 = "";
        }
        if(info.type >= 0){
            m_data5 = info.path  ? "Internet" : "Local";
            m_data6 = QString::number(info.frame_number) + "/" + QString::number(info.frame_total);
        }
        else{
            m_data5 = m_data6 = "";
        }
    }
    else if(m_protocol == "DMR"){
        m_data1 = m_dmrids[info.srcid];
        m_data2 = info.srcid ? QString::number(info.srcid) : "";
        m_data3 = info.dstid ? QString::number(info.dstid) : "";
        m_data4 = info.gwid ? QString::number(info.gwid) : "";
        QString s = "Slot" + QString::number(info.slot);
        QString flco;

        switch( (info.slot & 0x40) >> 6){
        case 0:
            flco = "Group";
            break;
        case 3:
            flco = "Private";
            break;
        case 8:
            flco = "GPS";
            break;
        default:
            flco = "Unknown";
            break;
        }

        if(info.frame_number){
            QString n = s + " " + flco + " " + QString("%1").arg(info.frame_number, 2, 16, QChar('0'));
            m_data5 = n;
        }
    }
    else if(m_protocol == "P25"){
        m_data1 = m_dmrids[info.srcid];
        m_data2 = info.srcid ? QString::number(info.srcid) : "";
        m_data3 = info.dstid ? QString::number(info.dstid) : "";
        m_data4 = info.srcid ? QString::number(info.srcid) : "";
        if(info.frame_number){
            QString n = QString("%1").arg(info.frame_number, 2, 16, QChar('0'));
            m_data5 = n;
        }
    }
    else if(m_protocol == "NXDN"){
        if(info.srcid){
            m_data1 = m_nxdnids[info.srcid];
            m_data2 = QString::number(info.srcid);
        }
        m_data3 = QString::number(info.dstid);

        if(info.frame_number){
            QString n = QString("%1").arg(info.frame_number, 4, 16, QChar('0'));
            m_data5 = n;
        }
    }
    else if(m_protocol == "M17"){
        m_data1 = info.src;
        m_data2 = info.dst + " " + info.module;
        m_data3 = info.type ? "3200 Voice" : "1600 V/D";
        if(info.frame_number){
            QString n = QString("%1").arg(info.frame_number, 4, 16, QChar('0'));
            m_data4 = n;
        }
        m_data5 = QString::number(info.streamid, 16);
    }
    else if(m_protocol == "IAX"){

    }
    QString t = QDateTime::fromMSecsSinceEpoch(info.ts).toString("yyyy.MM.dd hh:mm:ss.zzz");
    if((m_protocol == "DMR") || (m_protocol == "P25") || (m_protocol == "NXDN")){
        if(info.stream_state == Mode::STREAM_NEW){
            emit update_log(t + " " + m_protocol + " RX started id: " + " srcid: " + QString::number(info.srcid) + " dstid: " + QString::number(info.dstid));
        }
        if(info.stream_state == Mode::STREAM_END){
            emit update_log(t + " " + m_protocol + " RX ended id: " + " srcid: " + QString::number(info.srcid) + " dstid: " + QString::number(info.dstid));
        }
        if(info.stream_state == Mode::STREAM_LOST){
            emit update_log(t + " " + m_protocol + " RX lost id: " + " srcid: " + QString::number(info.srcid) + " dstid: " + QString::number(info.dstid));
        }
    }
    else{
        if(info.stream_state == Mode::STREAM_NEW){
            emit update_log(t + " " + m_protocol + " RX started id: " + QString::number(info.streamid, 16) + " src: " + info.src + " dst: " + info.gw2);
        }
        if(info.stream_state == Mode::STREAM_END){
            emit update_log(t + " " + m_protocol + " RX ended id: " + QString::number(info.streamid, 16) + " src: " + info.src + " dst: " + info.gw2);
        }
        if(info.stream_state == Mode::STREAM_LOST){
            emit update_log(t + " " + m_protocol + " RX lost id: " + QString::number(info.streamid, 16) + " src: " + info.src + " dst: " + info.gw2);
        }
    }
    emit update_data();
}

void Digihamlib::tts_changed(QString tts)
{
    if(tts == "Mic"){
        m_tts = 0;
    }
    else if(tts == "TTS1"){
        m_tts = 1;
    }
    else if(tts == "TTS2"){
        m_tts = 2;
    }
    else if(tts == "TTS3"){
        m_tts = 3;
    }
    else{
        m_tts = 0;
    }
    //  emit input_source_changed(m_tts, m_ttstxt);
}


void Digihamlib::tts_text_changed(QString ttstxt)
{
    m_ttstxt = ttstxt;
    //   emit input_source_changed(m_tts, m_ttstxt);
}

void Digihamlib::dtmf_send_clicked(QString dtmf)
{
    QByteArray tx(dtmf.simplified().toUtf8(), dtmf.simplified().size());
    //tx.prepend('*');
    //  emit send_dtmf(tx);
}

void Digihamlib::process_connect()
{
    if (connect_status != Mode::DISCONNECTED)
    {
        connect_status = Mode::DISCONNECTED;
        m_modethread->quit();
        m_data1.clear();
        m_data2.clear();
        m_data3.clear();
        m_data4.clear();
        m_data5.clear();
        m_data6.clear();
  //      emit connect_status_changed(0);
        updatelog("Disconnected");
    }
    else
    {
#ifdef Q_OS_IOS
        MicPermission::check_permission();
#endif
   //     emit connect_status_changed(1);
        connect_status = Mode::CONNECTING;
        QStringList sl;

        if (m_protocol != "IAX")
        {
            m_dstarusertxt.resize(20, ' ');
            m_host = m_hostmap[m_refname];
            printf("Host: %s\n", m_host.toLatin1().data());
            sl = m_host.split(',');

            if ((m_protocol == "M17") && !m_mdirect && (m_ipv6) && (sl.size() > 2) && (sl.at(2) != "none"))
            {
                m_host = sl.at(2).simplified();
                m_port = sl.at(1).toInt();
            }
            else if (sl.size() > 1)
            {
                m_host = sl.at(0).simplified();
                m_port = sl.at(1).toInt();
            }
            else if ((m_protocol == "M17") && m_mdirect)
            {
                qDebug() << "Going MMDVM_DIRECT";
            }
            else
            {
                m_errortxt = "Invalid host selection";
                connect_status = Mode::DISCONNECTED;
        //        emit connect_status_changed(5);
                return;
            }
        }

        QString vocoder = "";
        if ((m_vocoder != "Software vocoder") && (m_vocoder.contains(':')))
        {
            QStringList vl = m_vocoder.split(':');
            vocoder = vl.at(1);
        }
        QString modem = "";
        if ((m_modem != "None") && (m_modem.contains(':')))
        {
            QStringList ml = m_modem.split(':');
            modem = ml.at(1);
        }

    //    vocoder = "/dev/ttyUSB3"; // *************** take out

        const bool txInvert = true;
        const bool rxInvert = false;
        const bool pttInvert = false;
        const bool useCOSAsLockout = 0;
        const uint32_t ysfTXHang = 4;
        const float pocsagTXLevel = 50;
        const float m17TXLevel = 50;
        const bool duplex = m_modemRxFreq.toUInt() != m_modemTxFreq.toUInt();
        const int rxfreq = m_modemRxFreq.toInt() + m_modemRxOffset.toInt();
        const int txfreq = m_modemTxFreq.toInt() + m_modemTxOffset.toInt();

        emit update_log("Connecting to " + m_host + ":" + QString::number(m_port) + "...");

        uint16_t nxdnid = m_nxdnids.key(m_callsign);

        m_mode = Mode::create_mode(m_protocol);
        m_modethread = new QThread;
        m_mode->moveToThread(m_modethread);

        if (m_protocol == "IAX")
        {
            m_mode->set_iax_params(m_iaxuser, m_iaxpassword, m_iaxnode, m_iaxhost, m_iaxport);
            m_mode->init(m_callsign, m_dmrid, nxdnid, m_module, m_refname, m_iaxhost, m_iaxport, m_ipv6, vocoder, modem, m_mdirect);
            connect(this, SIGNAL(send_dtmf(QByteArray)), m_mode, SLOT(send_dtmf(QByteArray)));
        }
        else
        {
            m_mode->init(m_callsign, m_dmrid, nxdnid, m_module, m_refname, m_host, m_port, m_ipv6, vocoder, modem, m_mdirect);
        }

        m_mode->set_modem_flags(rxInvert, txInvert, pttInvert, useCOSAsLockout, duplex);
        m_mode->set_modem_params(m_modemBaud.toUInt(), rxfreq, txfreq, m_modemTxDelay.toInt(), m_modemRxLevel.toFloat(), m_modemRFLevel.toFloat(), ysfTXHang, m_modemCWIdTxLevel.toFloat(), m_modemDstarTxLevel.toFloat(), m_modemDMRTxLevel.toFloat(), m_modemYSFTxLevel.toFloat(), m_modemP25TxLevel.toFloat(), m_modemNXDNTxLevel.toFloat(), pocsagTXLevel, m17TXLevel);

        connect(this, SIGNAL(module_changed(char)), m_mode, SLOT(module_changed(char)));
        connect(m_mode, SIGNAL(process_audio(int16_t*,int)), this, SLOT(process_audio(int16_t*,int)));
        connect(m_mode, SIGNAL(update(Mode::MODEINFO)), this, SLOT(update(Mode::MODEINFO)));
        connect(m_mode, SIGNAL(update_log(QString)), this, SLOT(updatelog(QString)));
        connect(m_mode, SIGNAL(update_output_level(unsigned short)), this, SLOT(update_output_level(unsigned short)));
        connect(m_modethread, SIGNAL(started()), m_mode, SLOT(begin_connect()));
        connect(m_modethread, SIGNAL(finished()), m_mode, SLOT(deleteLater()));
//        connect(this, SIGNAL(input_source_changed(int,QString)), m_mode, SLOT(input_src_changed(int,QString)));
        connect(this, SIGNAL(swrx_state_changed(int)), m_mode, SLOT(swrx_state_changed(int)));
        connect(this, SIGNAL(swtx_state_changed(int)), m_mode, SLOT(swtx_state_changed(int)));
//        connect(this, SIGNAL(agc_state_changed(int)), m_mode, SLOT(agc_state_changed(int)));
        connect(this, SIGNAL(tx_clicked(bool)), m_mode, SLOT(toggle_tx(bool)));
        connect(this, SIGNAL(tx_pressed()), m_mode, SLOT(start_tx()));
        connect(this, SIGNAL(tx_released()), m_mode, SLOT(stop_tx()));
//        connect(this, SIGNAL(in_audio_vol_changed(qreal)), m_mode, SLOT(in_audio_vol_changed(qreal)));
        connect(this, SIGNAL(mycall_changed(QString)), m_mode, SLOT(mycall_changed(QString)));
        connect(this, SIGNAL(urcall_changed(QString)), m_mode, SLOT(urcall_changed(QString)));
        connect(this, SIGNAL(rptr1_changed(QString)), m_mode, SLOT(rptr1_changed(QString)));
        connect(this, SIGNAL(rptr2_changed(QString)), m_mode, SLOT(rptr2_changed(QString)));
        connect(this, SIGNAL(usrtxt_changed(QString)), m_mode, SLOT(usrtxt_changed(QString)));
        connect(this, SIGNAL(debug_changed(bool)), m_mode, SLOT(debug_changed(bool)));
        emit module_changed(m_module);
        emit mycall_changed(m_mycall);
        emit urcall_changed(m_urcall);
        emit rptr1_changed(m_rptr1);
        emit rptr2_changed(m_rptr2);
        emit usrtxt_changed(m_dstarusertxt);

        if (m_protocol == "DMR")
        {
            QString dmrpass = sl.at(2).simplified();

            if ((m_refname.size() > 2) && (m_refname.left(2) == "BM"))
            {
                if (!m_bm_password.isEmpty())
                {
                    dmrpass = m_bm_password;
                }
            }

            if ((m_refname.size() > 4) && (m_refname.left(4) == "TGIF"))
            {
                if (!m_tgif_password.isEmpty())
                {
                    dmrpass = m_tgif_password;
                }
            }
            m_mode->set_dmr_params(m_essid, dmrpass, m_latitude, m_longitude, m_location, m_description, m_freq, m_url, m_swid, m_pkgid, m_dmropts);
            connect(this, SIGNAL(dmr_tgid_changed(int)), m_mode, SLOT(dmr_tgid_changed(int)));
            connect(this, SIGNAL(dmrpc_state_changed(int)), m_mode, SLOT(dmrpc_state_changed(int)));
            connect(this, SIGNAL(slot_changed(int)), m_mode, SLOT(slot_changed(int)));
            connect(this, SIGNAL(cc_changed(int)), m_mode, SLOT(cc_changed(int)));
            emit dmr_tgid_changed(m_dmr_destid);
            emit dmrpc_state_changed(m_pc);
        }

        if (m_protocol == "M17")
        {
            connect(this, SIGNAL(m17_rate_changed(int)), m_mode, SLOT(rate_changed(int)));
            connect(this, SIGNAL(m17_can_changed(int)), m_mode, SLOT(can_changed(int)));
            if (m_mdirect)
            {
                connect(this, SIGNAL(dst_changed(QString)), m_mode, SLOT(dst_changed(QString)));
            }
        }

        m_modethread->start();
    }

    qDebug() << "process_connect called m_callsign == " << m_callsign;
    qDebug() << "process_connect called m_dmrid == " << m_dmrid;
    qDebug() << "process_connect called m_bm_password == " << m_bm_password;
    qDebug() << "process_connect called m_tgif_password == " << m_tgif_password;
    qDebug() << "process_connect called m_dmropts == " << m_dmropts;
    qDebug() << "process_connect called m_refname == " << m_refname;
    qDebug() << "process_connect called m_host == " << m_host;
    qDebug() << "process_connect called m_module == " << m_module;
    qDebug() << "process_connect called m_protocol == " << m_protocol;
    qDebug() << "process_connect called m_port == " << m_port;
}

void Digihamlib::process_audio(int16_t* data, int len)
{
    emit send_audio(data, len);
}

void Digihamlib::press_tx()
{
    emit tx_pressed();
}

void Digihamlib::release_tx()
{
    emit tx_released();
}
