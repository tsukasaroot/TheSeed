using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class profileManager : MonoBehaviour
{
    public GameObject player;
    public new Text name;
   
    void Start()
    {
    }

    // Start is called before the first frame update
    void OnEnable()
    {
        movementController p = player.GetComponent<movementController>();
        p.client.SendData("S_SENDPROFILE:" + p.client.nickName);
    }

    void Update()
    {
    }

    public void displayInventory(string[] informations)
    {
        name.text = informations[1];
    }
}
