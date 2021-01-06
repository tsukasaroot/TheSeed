using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Text;
using System;

public class movementController : MonoBehaviour
{
    public UDPClient UDPclient;
    public GameObject player;
    public GameObject HeadSet;
    private int i = 0;
    private string nickName;
    //private float movementSpeed;

    private UDPClient client;

    void Start()
    {
        client = Instantiate(UDPclient);
    }

    void Update()
    {
        string toExecute = client.ReceiveData();
        if (toExecute != null)
        {
            /*
            string[] isValidCommand = toExecute.Contains("movement") ? toExecute.Split(':') : null;

            if (isValidCommand.Length == 3 && isValidCommand[1] != null && isValidCommand[2] != null && isValidCommand[0] == "movement")
            {
            }*/
        }

        // Handle sent messages to server
        if (OVRInput.GetDown(OVRInput.Button.Two, OVRInput.Controller.LTouch))
        {
            client.SendData("getClientData:test");
        }
        /*
        Vector2 checkPosition;
        if (OVRInput.Get(OVRInput.Button.PrimaryThumbstickUp, OVRInput.Controller.LTouch))
            client.SendData("movement:forward:test");
        else if (OVRInput.Get(OVRInput.Button.PrimaryThumbstickDown, OVRInput.Controller.LTouch))
            client.SendData("movement:backward:test");
        else if (OVRInput.Get(OVRInput.Button.PrimaryThumbstickLeft, OVRInput.Controller.LTouch))
            client.SendData("movement:left:test");
        else if (OVRInput.Get(OVRInput.Button.PrimaryThumbstickRight, OVRInput.Controller.LTouch))
            client.SendData("movement:right:test");
            */
    }

    private void FixedUpdate()
    {
    }
}